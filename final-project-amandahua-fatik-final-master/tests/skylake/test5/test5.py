import os.path
import sys

event_dic = {"MEM": ['Memory read bandwidth [MBytes/s]', 'Memory write bandwidth [MBytes/s]',
                     'Memory read data volume [GBytes]', 'Memory write data volume [GBytes]'],  # rdtsc,core0,core1
             "L3CACHE": ['L3 miss rate STAT', 'L3 miss ratio STAT']  # rdtsc,min,max,avg
             }

nr_threads = 24
runtimes = ["OMP"]

threads = ['1', '2', '4', '6', '8', '10', '12', '14', '16', '18', '20', "22", "24"]
# threads = ['14','16','18','20',"22","24"]

app = "mg.C"

res_dic = {}
for runtime in runtimes:
    for thread in threads:
        key = runtime + "_" + thread
        res_dic[key] = [[[], [], []], [[],
                                       []]]  # [[rt],[core0:rd_bw,rd_vol,wr_bw,wr_vol],[core1:rd_bw,rd_vol,wr_bw,wr_vol]],[[rt],[l3mpki:min,max,avg,l3missratio:min,max,avg]]

for event, metrics in event_dic.items():
    for runtime in runtimes:
        for thread in threads:
            filename = app + "_" + thread + "_" + runtime + '_' + event + ".txt"
            key = runtime + "_" + thread

            if os.path.isfile(filename):
                file = open(filename, "r")

                for line in file:

                    try:
                        if event == "MEM":
                            if ("Runtime (RDTSC) [s]" in line) and int(thread) == 1:
                                data = line.strip().split('|')
                                while ("" in data):
                                    data.remove("")
                                res_dic[key][0][0].append(float(data[1]))  # avg

                            if "Runtime (RDTSC) [s] STAT" in line:
                                data = line.strip().split('|')
                                while ("" in data):
                                    data.remove("")
                                res_dic[key][0][0].append(float(data[4]))  # avg

                            for metric in metrics:
                                if metric in line and "STAT" not in line:
                                    data = line.strip().split('|')
                                    while ("" in data):
                                        data.remove("")

                                    res_dic[key][0][1].append(float(data[1]))  # core_0
                                    if int(thread) > (nr_threads / 2):
                                        res_dic[key][0][2].append(float(data[2]))  # core_1
                                    else:
                                        res_dic[key][0][2].append(0.0)

                        if event == "L3CACHE":
                            if "Runtime (RDTSC) [s] STAT" in line:
                                data = line.strip().split('|')
                                while ("" in data):
                                    data.remove("")
                                res_dic[key][1][0].append(float(data[4]))  # avg

                            if "L3 miss rate STAT" in line:
                                data = line.strip().split('|')
                                while ("" in data):
                                    data.remove("")
                                res_dic[key][1][1].append(float(data[2]) * 1000)  # min
                                res_dic[key][1][1].append(float(data[3]) * 1000)  # max
                                res_dic[key][1][1].append(float(data[4]) * 1000)  # avg

                            if "L3 miss ratio STAT" in line:
                                data = line.strip().split('|')
                                while ("" in data):
                                    data.remove("")
                                res_dic[key][1][1].append(float(data[2]))  # min
                                res_dic[key][1][1].append(float(data[3]))  # max
                                res_dic[key][1][1].append(float(data[4]))  # avg
                            if thread == "1":
                                if ("Runtime (RDTSC) [s]" in line):
                                    data = line.strip().split('|')
                                    while ("" in data):
                                        data.remove("")
                                    res_dic[key][1][0].append(float(data[1]))  # avg

                                if "L3 miss rate" in line:
                                    data = line.strip().split('|')
                                    while ("" in data):
                                        data.remove("")
                                    res_dic[key][1][1].append(float(data[1]) * 1000)  # avg

                                if "L3 miss ratio" in line:
                                    data = line.strip().split('|')
                                    while ("" in data):
                                        data.remove("")
                                    res_dic[key][1][1].append(float(data[1]))  # avg


                    except:
                        e = sys.exc_info()[0]
                        print("error: %s", e)
                        print(filename)

# [[rt],[core0:rd_bw,rd_vol,wr_bw,wr_vol],[core1:rd_bw,rd_vol,wr_bw,wr_vol]],[[rt],[l3mpki:min,max,avg,l3missratio:min,max,avg]]

# runtime
print('nr_threads,rt1,rt2')
for runtime in runtimes:
    for thread in threads:
        key = runtime + "_" + thread
        try:
            print("%s,%.2f,%.2f" % (thread, res_dic[key][0][0][0], res_dic[key][1][0][0]))
        except IndexError:
            pass

print('numOfThreads,readBW0,writeBW0,readBW1,writeBW1')
# memory bandwidth
for runtime in runtimes:
    for thread in threads:
        key = runtime + "_" + thread
        try:
            print("%s,%4.2f,%4.2f,%4.2f,%4.2f" % (
            thread, res_dic[key][0][1][0], res_dic[key][0][1][2], res_dic[key][0][2][0], res_dic[key][0][2][2]))
        except:
            pass
print('numOfThreads,readDataVolume0,writeDataVolume0,readDataVolume1,writeDataVolume1')
# memory bandwidth
for runtime in runtimes:
    for thread in threads:
        key = runtime + "_" + thread
        try:
            print("%s,%4.2f,%4.2f,%4.2f,%4.2f" % (
            thread, res_dic[key][0][1][1], res_dic[key][0][1][3], res_dic[key][0][2][1], res_dic[key][0][2][3]))
        except:
            pass

