import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np
import sys

df:pd.DataFrame

with open("out.txt") as fin:
    df = pd.read_csv(fin, sep="\t"
                     , comment="#"
                     , header=None
                     , names=["time", "caltime", "cmp", "iter", "memops", "vertices", "edges", "id"])

# ID is 1, 2 and 3 for worst, average and best cases 
# respectivly

df_worst = df[df["id"] == 1]
df_best = df[df["id"] == 3]
df_average = df[df["id"] == 2]

# Theoretical average
# C = 1/2 if int(sys.argv[1]) else 1 #BELLMAN-FORD
# f = lambda x: C * x * (x + 1) #BELLMAN-FORD
f_average = lambda x: 1/2 * x * (x + 1) * (x + 1) + x * x # TRANSITIVE CLOSURE
th_average = f_average(df_average["vertices"])

# Poly fit the "average" data points
p = np.poly1d(np.polyfit(df_average["vertices"]
                         , df_average["iter"]
                         , 2))
t = np.linspace(df_average["vertices"].min()
                , df_average["vertices"].max()
                , len(df_average["vertices"]))

# Theoretical Worst
f_worst = lambda x: x * (x + 1) * (x - 1) + x * x #TRANSITIVE CLOSURE
th_worst = f_worst(df_worst["vertices"]) #TRANSITIVE CLOSURE

plt.figure(1)

v = df_best["vertices"]

#Best case
plt.plot(v, df_best["iter"], "b-")

#Worst case
# plt.plot(v, df_worst["iter"], "r-")
plt.plot(v, th_worst, "r-")

#Average case
plt.scatter(v, df_average["iter"], marker=".", color="lightgreen")
plt.plot(v, p(t), "g--")
plt.plot(v, th_average, "g-")

plt.legend(["Best case",
            "Theoretical worst",
            "Average data points",
            "Polyfit of average",
            "Theoretical average"])

plt.title("Digrafo" if int(sys.argv[1]) else "Grafo")
plt.xlabel("|V|")
plt.ylabel("ITER");

#Save to figure
plt.savefig("out.png")
