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
def FigReadSpace(readfilename):
    all_data = []
    global y_max
    y_max = 1e-20
    data_len = sum([1 for _ in open(readfilename)])
    with open(readfilename) as f:
        for line in f:
            data_line = line.split(' ')
            data = []
            for val in data_line:
                if val != '\n' and val != '':
                    if val == '0':
                        data.append(0)
                    else:
                        if args.log and data_len > 1:
                            data.append(np.log10(int(val)))
                        else:
                            data.append(int(val))
                        y_max = max(y_max, int(val))
            all_data.append(data)
    return np.array(all_data)
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
def FigBin():
    dshape = data_bin.shape
    if dshape[0] == 1:
        fig, fig1  = FigGet(ver=10, hor=5) # Set fig
        FigParams()                        # Set params
        fig1.set_xlabel(r'Each bin size = {} [$\mu$s]'.format(2**args.power), fontsize=25, math_fontfamily='cm')
        
        bins = np.arange(0, len(data_bin[0]))
        if args.log:
            plt.yscale('log')
            fig1.set_ylabel(r'Frequency', fontsize=25, math_fontfamily='cm')
            fig1.set_ylim([1,y_max*10])
            outfilename  = "./output/png/lastbin{}_{}powerlog.png".format(args.time, args.power)
        else:
            fig1.set_ylabel('Frequency', fontsize=25)
            fig1.set_ylabel(r'Frequency', fontsize=25, math_fontfamily='cm')
            outfilename  = "./output/png/lastbin{}_{}power.png".format(args.time, args.power)
        plt.bar(bins, data_bin[0])
        
        plt.savefig(outfilename, bbox_inches="tight", pad_inches=0.05)
    else:
        time_step = 0
        fig = plt.figure(figsize=(10,10))
        FigParams()                        # Set params

        plt.rcParams['axes.labelpad'] = 15
        #print(rcParams.keys())
        """
        for ii in rcParams.keys():
            if 'scale' in ii:
                print(ii)
        """
        fig1 = fig.add_subplot(111, projection='3d', computed_zorder=False)
        fig1.set_xlabel(r'Each bin size = {} [$\mu$s]'.format(2**args.power), fontsize=25, math_fontfamily='cm')
        fig1.set_ylabel('Timestep', fontsize=25)
        if args.log:
            fig1.set_zlabel(r'Frequency ($10^z$)', fontsize=25, math_fontfamily='cm')
        else:
            fig1.set_zlabel(r'Frequency', fontsize=25, math_fontfamily='cm')
            fig1.ticklabel_format(style="sci", axis='z', scilimits=(0,0))
        fig1.view_init(elev=30, azim=-60)
        
        for one_data in data_bin:
            bins = np.arange(0, len(one_data))
            plt.bar(bins, one_data, zs=time_step, zorder=-time_step, zdir='y', color='red', edgecolor='k', linewidth=0.2)
            time_step += 1
        if args.log:
            outfilename  = "./output/png/allbin{}_{}powerlog.png".format(args.time, args.power)
        else:
            outfilename  = "./output/png/allbin{}_{}power.png".format(args.time, args.power)
        plt.savefig(outfilename, bbox_inches="tight", pad_inches=0.8)

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
    FigMAPlot(window_size=10)  # Plot the result of moving average
    FigMAPlot(window_size=50)  # Plot the result of moving average
    #FigMAPlot(window_size=100)  # Plot the result of moving average
    FigMAPlot(window_size=500)  # Plot the result of moving average
    FigMAPlot(window_size=1000) # Plot the result of moving average
    #plt.show()
    fig1.legend(ncol=4)         # Number of legends
    plt.savefig(outfilename, bbox_inches="tight", pad_inches=0.05)
    
    readfilename_bin = "./output/csv/bin{}_{}power.csv".format(args.time, args.power)
    data_bin   = FigReadSpace(readfilename_bin)
    FigBin()

    readfilename_bin = "./output/csv/allbin{}_{}power.csv".format(args.time, args.power)
    data_bin   = FigReadSpace(readfilename_bin)
    FigBin()
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser("Output the result using matplotlib.")
    parser.add_argument("-t", "--time", help="Observation time, report time", default=1, type=float)
    parser.add_argument("-v", "--verbose", help="verbose mode", default=0, type=int)
    parser.add_argument("-p", "--power", help="What is the powr of 2?", default=1, type=int)
    parser.add_argument("-l", "--log", help="log", default=False, action='store_true')
    args = parser.parse_args()
    if args.time >= 1:
        args.time = int(args.time)
    main(args)