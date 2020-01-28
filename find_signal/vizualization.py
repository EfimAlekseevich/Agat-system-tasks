import matplotlib.pyplot as plt
import matplotlib.ticker as ticker


items = list()

def get_absolute_correlation(s1, s2):
    corr = 0
    for i in range(len(s1)):
        corr += s1[i] * s2[i]

    return corr



def get_correlations(nums, sync):
    correlations = []
    for i in range(len(nums)-len(sync)+1):
        correlations.append(get_absolute_correlation(nums[i:i+len(sync)], sync) / get_absolute_correlation(sync, sync))
        if correlations[i] > 50:
            items.append(i/24000)
    return correlations


def get_norm_correlations(nums, sync):
    correlations = []
    for i in range(len(nums)-len(sync)+1):
        signal = nums[i:i+len(sync)]
        correlations.append(get_absolute_correlation(signal, sync) /
        (get_absolute_correlation(signal, signal) * get_absolute_correlation(sync, sync)) ** 0.5 )
        if correlations[i] > 0.75:
            items.append(i)
    return correlations


def get_nums(filename):
    with open(filename, 'r') as data_file:
        data = data_file.readlines()
        nums = map(int, data)

    return list(nums)


def main():
    filename = 'real_rec.txt'

    print('\rReading data')
    nums = get_nums(filename)

    sync = [
          170, 233, 281, 309, 318, 312, 300, 290, 287, 290, 294, 291, 277, 256, 236, 228, 242, 279,
          329, 374, 392, 366, 289, 167, 20, -125, -237, -291, -271, -181, -39, 124, 275, 386, 440, 433,
          374, 277, 157, 29, -93, -200, -287, -347, -379, -378, -343, -275, -179, -63, 62, 182, 287,
          366, 409, 411, 369, 285, 170, 39, -90, -203, -288, -342, -363, -353, -319, -271, -222, -190,
          -190, -226, -295, -375, -438, -455, -406, -288, -119, 64, 219, 305, 301, 208, 56, -109, -234,
          -281, -234, -109, 56, 209, 301, 305, 218, 64, -115, -276, -384, -424, -406, -351, -290, -246,
          -228, -235, -256, -278, -291, -294, -290, -287, -290, -300, -312, -318, -309, -281, -233, -170
          ]


    print('\r\nProcessing data')
    correlations = get_correlations(nums, sync)

    print('\r\nOutput info')
    print(len(items))
    print(items)


    fig1, ax1 = plt.subplots()

    ax1.plot(sync)
    fig1.savefig('Sync120.png')
    plt.show()

    start = 2400
    stop = 7200

    fig2, (ax21, ax22) = plt.subplots(nrows=2, ncols=1)

    ax21.plot(range(start, stop), nums[start:stop])
    ax21.set_title('Signal')

    ax22.plot(range(start, stop), correlations[start:stop])
    ax22.set_title('Correlations')
    #ax22.yaxis.set_major_locator(ticker.MultipleLocator(0.1))
    #ax22.yaxis.set_minor_locator(ticker.MultipleLocator(0.05))

    fig2.savefig(f'Sign_corr{start}-{stop}.png')
    plt.show()



if __name__ == "__main__":
     main()
