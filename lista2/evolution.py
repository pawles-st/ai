from itertools import combinations, permutations
import numpy as np
import os
import random
from subprocess import Popen
import time

depth = 5
params = 4 * 2

population_size = 10
port = 1200

surviving = 2
mutation_chance = 0.05

def run(population, statistics, pair):
    global port
    i = pair[0]
    j = pair[1]
    print(pair)
#    print(f"port: {port}")
#    print(f"bot 1: {population[i]} vs bot 2: {population[j]}")
    server = Popen(f"./game_server 127.0.0.1 {port} >/dev/null", shell = True, preexec_fn=os.setsid)
    time.sleep(0.2)
    bot1 = Popen(f"./game_client_bot 127.0.0.1 {port} 1 {depth} {population[i][0]} {population[i][1]} {population[i][2]} {population[i][3]} {population[i][4]} {population[i][5]} {population[i][6]} {population[i][7]}", shell = True, preexec_fn=os.setsid)
    bot2 = Popen(f"./game_client_bot 127.0.0.1 {port} 2 {depth} {population[j][0]} {population[j][1]} {population[j][2]} {population[j][3]} {population[j][4]} {population[j][5]} {population[j][6]} {population[j][7]}", shell = True, preexec_fn=os.setsid)
#    print("made server")
    server.wait()
#    print(server.returncode)
    if server.returncode == 0:
        bot1.wait()
#        print(bot1.returncode)
        bot2.wait()
#        print(bot2.returncode)
    if server.returncode != 0:
#        raise ConnectionError("can't connect")
        port += 1
#        print("trying again on a new port")
#        print(port)
        run(population, statistics, pair)
    else:
        if bot1.returncode == 1:
            statistics[i][0] += 1
        else:
            statistics[j][1] += 1
        print(f"statistics: {statistics}")
#        print(f"bot 1 -> {bot1.returncode}")
#        print(f"bot 2 -> {bot2.returncode}")
        port += 1

if __name__ == "__main__":
    out = open("evolution.txt", "w")
    rnd = 1 # current round
    population = [[random.uniform(-10, 10) for _ in range(params)] for _ in range(population_size)]
#    population = [[2.7938269918857817, -7.903105284343317, -3.247046705872724, -0.3501449763306154, 4, -2, -3, 3], [7.125968707662774, -5.811613066676207, 2.5472432528131894, 0.1585854702744242, 0.8032465296105856, -0.868780037778538, 1.0685049970061056, -1.3335987725581242], [5.477424687855858, -5.812512827172014, 4.226265806720565, 2.6439779769585354, -0.10832036927279493, 0.09759076755521878, 1.5835432338051816, -2.4732083340904016], [7.286153565881008, -5.8113632384558045, 2.5084315205941743, 1.6386695815373122, -1.843022059577268, -3.330663629376664, -0.6189113159792696, 1.527381307142718]] + [[random.uniform(-10, 10) for _ in range(params)] for _ in range(population_size - 4)]
#    population = [[7.220924780103783, -5.811550939059185, 2.518523615622685, 0.11865303630363311, 4, -2, -3, 3], [7.474367479210672, -5.811467458282348, 2.329280081444173, 0.35305280379097836, 0.6010393385414636, -1.2106947406467412, 1.068468297061576, -1.3414896072819285], [7.203065639417419, -5.811390093948094, 2.1507881329459653, 0.032396681463471036, 1.0146467891089344, -1.0785692266304285, 1.068277698405988, -1.3359033619217011], [7.235883863657225, -5.811372596302655, 2.5377627554854096, 0.15578669477718599, 0.5095874767191646, -1.2131072365660331, -3.4577385442167694, -1.314483887033996], [7.295619134991261, -5.8114044139821965, 2.2504762166730137, -0.5996073127800521, 1.0361851373289812, -1.203630587619567, 1.068384550605832, -1.338962150643878], [7.202327442645895, -8.29419456726944, 2.6735018769164984, 0.20457616047212096, 0.7784884802876222, -1.1051968449801188, 1.0683901273224012, -1.3319656153270047], [7.293366713452129, -5.8113445771618535, 2.5000283316824596, 1.6538333683147821, 0.13771881513757445, -0.5416428312290443, 1.0685353402422249, -1.3511162884846935], [7.203434613520786, -5.811276651727428, 1.390014774501499, 0.014786101823677279, 0.6757830057678582, -1.2760783426917819, 1.0683634920667022, -1.3347041126136652], [7.340298958213605, -5.811421326650807, 2.7593443366826236, -1.0733982621290763, 1.1697628785546874, -1.3013184094720367, 1.0682122121718542, -1.3270171175935597], [7.229149528680593, -5.811368347316799, 1.5099798531113353, -0.026594528491448728, 0.9213177163993419, -1.2449034710607472, 1.0683585558124544, -1.346048352234767]]

    while True:
        random.shuffle(population)
        statistics = [[0, 0] for _ in range(population_size)] # (X_wins, O_wins)
        print(population)
        for pair in permutations(range(population_size), 2):
            run(population, statistics, pair);
        out.write("--> " + str(rnd) + " <--\n")
        out.write(str(population) + "\n")
        out.write(str(statistics) + "\n\n")

        scoresX = sorted(range(population_size), key = lambda x : statistics[x][0], reverse = True)
#        print("scoresX")        
#        print(scoresX)
        ppbX = [s[0] ** 3 for s in statistics]
        if all([x == 0 for x in ppbX]):
            ppbX = [x + 1 for x in ppbX]
#        print("ppbX")
#        print(ppbX)
        totalX = sum(ppbX)
#        print("totalX")
#        print(totalX)

        scoresO = sorted(range(population_size), key = lambda x : statistics[x][1], reverse = True)
#        print("scoresO")
#        print(scoresO)
        ppbO = [s[1] ** 3 for s in statistics]
        if all(o == 0 for o in ppbO):
            ppbO = [o + 1 for o in ppbO]
#        print("ppbO")
#        print(ppbO)
        totalO = sum(ppbO)
#        print("totalO")
#        print(totalO)

        new_population = [population[scoresX[i]][0:params // 2] + population[scoresO[i]][params // 2:params] for i in range(surviving)]
#        print(new_population)

        # cross

        while len(new_population) < population_size:
            
            individual = []

            # pick X parents

            p1 = p2 = 0
            while p1 == p2:

                # p1

                p = random.randint(0, totalX)
                x = ppbX[0]
                i = 0
                while x < p:
                    i += 1
                    x += ppbX[i]
                p1 = i

                # p2

                p = random.randint(0, totalX)
                x = ppbX[0]
                i = 0
                while x < p:
                    i += 1
                    x += ppbX[i]
                p2 = i

            for param in range(params // 2):
                w = random.random()
                individual.append(population[p1][param] * w + population[p2][param] * (1 - w))

            # pick O parents

            p1 = p2 = 0
            while p1 == p2:

                # p1

                p = random.randint(0, totalO)
                x = ppbO[0]
                i = 0
                while x < p:
                    i += 1
                    x += ppbO[i]
                p1 = i

                # p2

                p = random.randint(0, totalO)
                x = ppbO[0]
                i = 0
                while x < p:
                    i += 1
                    x += ppbO[i]
                p2 = i

            for param in range(params // 2, params):
                w = random.random()
                individual.append(population[p1][param] * w + population[p2][param] * (1 - w))

            new_population.append(individual)

#        print("new: ")
#        print(new_population)

        # mutate

        for i in range(population_size):
            for param in range(params):
                p = random.random()
                if p <= mutation_chance:
                    new_population[i][param] += random.uniform(-5, 5)
        population = new_population
        rnd += 1
