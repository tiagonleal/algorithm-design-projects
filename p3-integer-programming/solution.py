from pulp import *

n, m = map(int, input().split())
jogos_passados = set()
jogos_futuros = []
pontos_n = [0] * (n + 1)

for _ in range(m):
    a, b, result = map(int, input().split())
    jogos_passados.add((a, b))
    if result == a:
        pontos_n[a] += 3
    elif result == b:
        pontos_n[b] += 3
    else:
        pontos_n[a] += 1
        pontos_n[b] += 1

for i in range(1, n + 1):
    for j in range(1, n + 1):
        if i != j and (i, j) not in jogos_passados:
            jogos_futuros.append((i, j))

# Calcular pontos máximos possíveis para cada equipa
max_pontos = list(pontos_n)
for (i, j) in jogos_futuros:
    max_pontos[i] += 3
    max_pontos[j] += 3

for equipa in range(1, n + 1):
    # Verificar se é impossível (algum adversário já tem mais pontos do que o máximo possível da equipa)
    impossivel = False
    for adv in range(1, n + 1):
        if adv != equipa and pontos_n[adv] > max_pontos[equipa]:
            impossivel = True
            break
    if impossivel:
        print(-1)
        continue
    
    # Se não há jogos futuros, verificar se já está empatado ou à frente
    if not jogos_futuros:
        pode_ganhar = True
        for adv in range(1, n + 1):
            if adv != equipa and pontos_n[equipa] < pontos_n[adv]:
                pode_ganhar = False
                break
        print(0 if pode_ganhar else -1)
        continue

    prob = LpProblem("p", LpMinimize)

    vit = {}
    der = {}
    emp = {}
    
    for (i, j) in jogos_futuros:
        vit[(i,j)] = LpVariable(f"v{i}_{j}", cat='Binary')
        der[(i,j)] = LpVariable(f"d{i}_{j}", cat='Binary')
        emp[(i,j)] = LpVariable(f"e{i}_{j}", cat='Binary')
        prob += vit[(i,j)] + der[(i,j)] + emp[(i,j)] == 1

    pontos_finais = list(pontos_n)
    
    for (i, j) in jogos_futuros:
        pontos_finais[i] += 3 * vit[(i,j)] + emp[(i,j)]
        pontos_finais[j] += 3 * der[(i,j)] + emp[(i,j)]

    vitorias_alvo = []
    for (i, j) in jogos_futuros:
        if i == equipa:
            vitorias_alvo.append(vit[(i,j)])
        if j == equipa:
            vitorias_alvo.append(der[(i,j)])

    prob += lpSum(vitorias_alvo)

    for adv in range(1, n + 1):
        if adv != equipa:
            prob += pontos_finais[equipa] >= pontos_finais[adv]

    prob.solve(GLPK_CMD(msg=0))
    
    if prob.status == 1:
        obj = value(prob.objective)
        if obj is not None:
            print(int(obj))
        else:
            print(0)
    else:
        print(-1)