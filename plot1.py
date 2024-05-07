import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

import argparse
import time
import json

def FigParams():
    plt.rcParams['font.size']            = 25
    plt.rcParams['xtick.labelsize']      = 20
    plt.rcParams['ytick.labelsize']      = 20
    plt.rcParams['legend.fontsize']      = 15
    plt.rcParams['legend.fancybox']      = False
    plt.rcParams['legend.framealpha']    = 1
    plt.rcParams['legend.edgecolor']     = 'black'
    plt.rcParams['legend.handletextpad'] = 1.5
    plt.rcParams['xtick.direction']      = 'inout'
    plt.rcParams['ytick.direction']      = 'inout'
    plt.rcParams['font.family']          = 'Times'#'DejaVu Serif'
    plt.rcParams['pdf.fonttype']         = 42
    plt.rcParams['axes.axisbelow']       = True

def FigReadJson(readfilename):
    tf = open(readfilename, 'r')
    data = json.load(tf)
    return data
def FigRead(readfilename):
    data = np.loadtxt(readfilename, comments='#', delimiter=',')
    return data    
def FigGet(ver, hor):
    fig = plt.figure(figsize=(ver,hor))
    fig1 = fig.add_subplot(111)

    fig1.spines["top"].set_linewidth(2)
    fig1.spines["right"].set_linewidth(2)
    fig1.spines["bottom"].set_linewidth(2)
    fig1.spines["left"].set_linewidth(2)
    fig1.grid(color='black', linestyle='dotted', linewidth=1)
    fig1.tick_params(width=2, length=8, labelsize=25)
    return fig, fig1

def FigPlot():
    x = data[:,1]
    y = data[:,2]
    fig1.set_xlabel('Time step', fontsize=25)
    fig1.set_ylabel('# of packets', fontsize=25)

    ## Setting X axis limit
    """
    x_max = max(x)
    test = [(x_max + i) // 5 for i in range(5)]
    test_2 = round(test[len(test)-1],-1) 
    try:
        my_range = np.arange(0,x_max+test_2,test_2)
    except:
        my_range = np.arange(0,x_max+test_2)
    
    fig1.xaxis.set_ticks(my_range)
    """
    
    ## Setting Y axis limit
    y_max = max(y)
    fig1.set_ylim(0,y_max+(y_max/4))
    
    fig1.plot(x, y, label='Non-MA')

def FigMAPlot(window_size=10):
    x = data[:,1]
    y = data[:,2]
    if len(x) <= window_size:
        return
    mv_filter = np.ones(window_size)/window_size
    
    y_mv = np.convolve(y, mv_filter, mode='valid')
    x_mv = np.arange(len(y_mv))

    fig1.plot(x_mv, y_mv, label='WS={}'.format(window_size))
def FigBar():
    global args
    fig = plt.figure(figsize=(7,7))
    ax = fig.add_subplot(111, projection='3d')
    bin_size = 16
    iat_max = 0
    for i in data_bin.keys():
        tmp_max = max(data_bin[i])
        iat_max = max(tmp_max, iat_max)
    time_bin = np.arange(0,iat_max,)
    for i in data_bin.keys():
        try:
            ## Inter-arival time
            iat = data_bin[i]
            my_time = np.full(len(data_bin[i]), float(i))
            hist, xedges, yedges = np.histogram2d(my_time, iat, bins=10)

            xpos, ypos = np.meshgrid(yedges[:-1], xedges[:-1])
            zpos = 0
            dx = (xpos[0][1] - xpos[0][0])/10
            dy = (ypos[1][0] - ypos[0][0])/100
            dz = hist.ravel()

            xpos = xpos.ravel()
            ypos = ypos.ravel()

            ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color='red')
        except KeyboardInterrupt:
            print(" stop.")
            #plt.show()
            #return fig, ax
            break
    outfilename  = "./output/png/bin{}.png".format(args.time)
    print("Take many time...")
    plt.savefig(outfilename, bbox_inches="tight", pad_inches=0.05)
    #return fig, ax
## -----------------------------------------------------------------------
def main(args):
    global data, data_bin, fig, fig1
    readfilename = "./output/csv/time_pkts{}.csv".format(args.time)
    outfilename  = "./output/png/time_pkts{}.png".format(args.time)
    print("read: {}\nout : {}".format(readfilename, outfilename))
    
    FigParams()                        # Set params
    data       = FigRead(readfilename) # Read and get data
    fig, fig1  = FigGet(ver=10, hor=5) # Set fig
    FigPlot()                   # Plot raw results
    FigMAPlot(window_size=100)  # Plot the result of moving average
    FigMAPlot(window_size=500)  # Plot the result of moving average
    FigMAPlot(window_size=1000) # Plot the result of moving average
    #plt.show()
    fig1.legend(ncol=4)         # Number of legends
    plt.savefig(outfilename, bbox_inches="tight", pad_inches=0.05)
    

    if args.verbose:
        readfilename_bin = "./output/csv/binraw{}.json".format(args.time)
        outfilename  = "./output/png/bin{}.png".format(args.time)
        data_bin   = FigReadJson(readfilename_bin)
        #fig, fig1 = FigBar()
        FigBar()
        #plt.show()
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser("Output the result using matplotlib.")
    parser.add_argument("-t", "--time", help="Observation time, report time", default=1, type=float)
    parser.add_argument("-v", "--verbose", help="verbose mode", default=0, type=int)
    args = parser.parse_args()
    if args.time >= 1:
        args.time = int(args.time)
    main(args)