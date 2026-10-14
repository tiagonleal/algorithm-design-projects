import subprocess
import time
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os
import sys

# --- CONFIGURAÇÕES ---
NUM_INSTANCES = 15      # Quantos testes queres fazer
START_N = 10            # Começar com 10 equipas
STEP_N = 5              # Aumentar 5 equipas por iteração (vai até 10 + 5*14 = 80)
PROBABILITY = 50        # Probabilidade de jogos já realizados (50%)
PROJECT_FILE = "project3.py"
GENERATOR_SOURCE = "gerador_p3.cpp"
GENERATOR_EXE = "gerador"
OUTPUT_CSV = "dados_complexidade.csv"
OUTPUT_PLOT = "grafico_complexidade.png"

def compile_generator():
    print("🔨 A compilar o gerador C++...")
    try:
        subprocess.run(["g++", "-o", GENERATOR_EXE, GENERATOR_SOURCE], check=True)
    except subprocess.CalledProcessError:
        print("❌ Erro ao compilar o gerador. Verifica se tens o g++ instalado.")
        sys.exit(1)

def run_benchmark():
    results = []
    print(f"\n🚀 A começar o benchmark com {NUM_INSTANCES} instâncias...")
    print(f"{'N (Equipas)':<15} | {'Tempo (s)':<15}")
    print("-" * 35)
    
    for i in range(NUM_INSTANCES):
        n = START_N + i * STEP_N
        
        # 1. Gerar o input chamando o executável C++
        # O gerador imprime para o stdout
        try:
            process_gen = subprocess.run(
                [f"./{GENERATOR_EXE}", str(n), str(PROBABILITY)], 
                capture_output=True, text=True, check=True
            )
            input_data = process_gen.stdout
        except subprocess.CalledProcessError:
            print(f"❌ Erro ao gerar instância para N={n}")
            continue
        
        # 2. Correr o projeto Python e medir o tempo
        start_time = time.time()
        try:
            # Passamos o input_data via stdin para o teu script
            process_proj = subprocess.run(
                ["python3", PROJECT_FILE], 
                input=input_data, text=True, capture_output=True
            )
            
            # Verificar se deu erro no script (ex: erro do GLPK)
            if process_proj.returncode != 0:
                print(f"⚠️ Erro no project3.py com N={n}. Verifica se tens o GLPK instalado.")
                print(process_proj.stderr)
                
        except Exception as e:
            print(f"Erro a executar o projeto: {e}")
            continue
            
        end_time = time.time()
        elapsed = end_time - start_time
        
        print(f"{n:<15} | {elapsed:.4f}s")
        results.append({"N": n, "Tempo": elapsed})

    return pd.DataFrame(results)

def plot_results(df):
    if df.empty:
        print("⚠️ Sem dados para plotar.")
        return

    plt.figure(figsize=(10, 6))
    
    # Pontos reais
    plt.plot(df["N"], df["Tempo"], 'o-', label="Tempo Real (Medido)", color='#1f77b4')
    
    # Linha de tendência cúbica (O(n^3)) para comparação teórica
    if len(df) > 3:
        # Fit polinomial de grau 3
        p = np.polyfit(df["N"], df["Tempo"], 3)
        f = np.poly1d(p)
        x_new = np.linspace(df["N"].min(), df["N"].max(), 100)
        plt.plot(x_new, f(x_new), '--', label="Ajuste Polinomial ($O(n^3)$)", color='#ff7f0e')

    plt.title("Análise de Complexidade: Resolução LP", fontsize=14)
    plt.xlabel("Número de Equipas (N)", fontsize=12)
    plt.ylabel("Tempo de Execução (segundos)", fontsize=12)
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.savefig(OUTPUT_PLOT)
    print(f"\n📈 Gráfico guardado como: {OUTPUT_PLOT}")

if __name__ == "__main__":
    # Verificar ficheiros
    if not os.path.exists(PROJECT_FILE):
        print(f"❌ Erro: O ficheiro '{PROJECT_FILE}' não está nesta pasta.")
        sys.exit(1)
    if not os.path.exists(GENERATOR_SOURCE):
        print(f"❌ Erro: O ficheiro '{GENERATOR_SOURCE}' não está nesta pasta.")
        sys.exit(1)

    # Executar pipeline
    compile_generator()
    df = run_benchmark()
    
    if not df.empty:
        # Exportar CSV para Excel
        df.to_csv(OUTPUT_CSV, index=False)
        print(f"💾 Dados exportados para: {OUTPUT_CSV} (Podes abrir no Excel)")
        
        plot_results(df)
        print("\n✅ Tudo feito! Agora vai beber um café.")
