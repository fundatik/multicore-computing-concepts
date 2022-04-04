threads = ['1','2','4','6','8','10','12','14','16','18','20',"22","24"]
runtimes = ["CLK","OMP","FF"]
app = "mg.C"

res_dic = {}
for runtime in runtimes:
    for thread in threads:
        key = runtime+"_"+thread
        res_dic[key] = []

for runtime in runtimes:
    for thread in threads:
        filename = app + "_" + thread + "_" + runtime + ".txt"
        file = open(filename, "r")
        for line in file:
            if "Time in seconds" in line:
                line = line.split("                  ")
                key = runtime + "_" + thread
                res_dic[key].append(str(round(float(line[1]),2)))

for thread in threads:    
    res = ''
    for runtime in runtimes:
        key = runtime + "_" + thread
        res = res + res_dic[key][0] + " "
    print(thread, res)

