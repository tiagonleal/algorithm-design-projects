#!/bin/bash

# Script de análise PARALELA - MEGA RÁPIDO

if [ ! -f "./gerador_p2" ]; then
    g++ -std=c++11 -O3 -Wall gerador_p2.cpp -o gerador_p2 -lm
fi

if [ ! -f "./project02" ]; then
    g++ -std=c++11 -O3 -Wall project02.cpp -o project02 -lm
fi

mkdir -p test_results
RESULTS_FILE="test_results/dados.txt"
TEMP_DIR="test_results/temp"
mkdir -p $TEMP_DIR

# Limpa ficheiros temporários anteriores
rm -f $TEMP_DIR/*.txt

sizes=(100 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500)

echo "Executando TODOS os testes em PARALELO..."
echo "Aguarde..."

# Função para executar um teste
run_test() {
    n=$1
    run=$2
    
    # Gera input
    ./gerador_p2 $n 10 95 $((42 + run)) > $TEMP_DIR/input_${n}_${run}.txt 2>/dev/null
    
    # Mede tempo
    start=$(date +%s%N)
    ./project02 < $TEMP_DIR/input_${n}_${run}.txt > /dev/null 2>&1
    end=$(date +%s%N)
    
    tempo=$((($end - $start) / 1000000))
    
    # Salva resultado temporário
    echo "$tempo" > $TEMP_DIR/result_${n}_${run}.txt
}

# Exporta função para subshells
export -f run_test

# Lança TODOS os testes em paralelo
for n in "${sizes[@]}"; do
    for run in 1 2 3 4 5; do
        run_test $n $run &
    done
done

# Aguarda todos terminarem
wait

echo "Todos os testes concluídos! Processando resultados..."

# Processa resultados
echo "n,tempo_ms,n_quadrado,n_cubo" > $RESULTS_FILE

for n in "${sizes[@]}"; do
    # Coleta os 5 tempos
    times=()
    for run in 1 2 3 4 5; do
        if [ -f "$TEMP_DIR/result_${n}_${run}.txt" ]; then
            t=$(cat $TEMP_DIR/result_${n}_${run}.txt)
            times+=($t)
        fi
    done
    
    # Calcula mediana (ordena e pega elemento do meio)
    IFS=$'\n' sorted=($(printf '%s\n' "${times[@]}" | sort -n))
    median=${sorted[2]}
    
    n2=$((n * n))
    n3=$((n * n * n))
    
    echo "$n,$median,$n2,$n3" >> $RESULTS_FILE
done

echo ""
echo "=== RESULTADOS ==="
cat $RESULTS_FILE

# Limpa temporários
rm -rf $TEMP_DIR

echo ""
echo "Ficheiro salvo: $RESULTS_FILE"