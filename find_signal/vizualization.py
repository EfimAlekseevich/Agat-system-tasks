import matplotlib.pyplot as plt


def get_nums(filename):
    with open(filename, 'r') as data_file:
        data = data_file.readlines()
        nums = map(int, data)

    return list(nums)


def main():
    filename = 'real_rec.txt'
    nums = get_nums(filename)

    sync = [
          -170, -233, -281, -309, -318, -312, -300, -290, -287, -290, -294, -291, -278, -256, -235,
          -228, -246, -290, -351, -406, -424, -384, -276, -115, 64, 218, 305, 301, 209, 56, -109, -234,
          -281, -234, -109, 56, 208, 301, 305, 219, 64, -119, -288, -406, -455, -438, -375, -295, -226,
          -190, -190, -222, -271, -319, -353, -363, -342, -288, -203, -90, 39, 170, 285, 369, 411, 409,
           366, 287, 182, 62, -63, -179, -275, -343, -378, -379, -347, -287, -200, -93, 29, 157, 277,
           374, 433, 440, 386, 275, 124, -39, -181, -271, -291, -237, -125, 20, 167, 289, 366, 392, 374,
           329, 279, 242, 228, 236, 256, 277, 291, 294, 290, 287, 290, 300, 312, 318, 309, 281, 233, 170, ]

    fig, ax = plt.subplots()

    plt.plot(sync)
    plt.show()
    fig.savefig('Sync.png')


    plt.plot(nums[:10000])
    plt.show()
    #plt.savefig('Signal')


if __name__ == "__main__":
     main()
