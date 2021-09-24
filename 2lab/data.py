import matplotlib.pyplot as plt
import numpy as np

def task_1():
    # 10 100 1000 10000 100000 1000000 10000000 100000000 
    # 0 0.01 0.009 0.0102 0.0097 0.009845 0.0098831 0.00990391 
    # 0.1 0.09 0.096 0.1079 0.1071 0.108619 0.10882 0.108914 
    # 0 0.03 0.091 0.0942 0.09764 0.099066 0.0989099 0.0990448 
    out = open("C:/0.LaLaLand/0.Phystech/2kurs/out1.txt", 'r')

    test_count = np.array(list(map(float, out.readline().strip().split(' '))))

    prob = [1/100, 11/100, 10/100]

    name = ['Discrete{1}', 'Segment{0, 10}', 'Set{1, 3, 5, 7, 23, 48, 57, 60, 90, 99}']
    name1 = ['Discrete', 'Segment', 'Set']

    for i in range(3):
        tmp = np.array(list(map(float, out.readline().strip().split(' '))))
        fig, ax = plt.subplots()
        ax.grid(True)
        # ax.set(xlabel='N', ylabel='t',
        #        title='Полная энергия системы')
        ax.set(ylabel='$p$', xlabel='lg(test count)',
            title=name[i])
        ax.plot(np.log10(test_count), tmp, label='$p_{exp}$')
        ax.scatter(np.log10(test_count), tmp)
        ax.plot(np.log10(test_count), np.zeros(len(test_count))+prob[i], label='$p_{teor}$')
        plt.legend()
        fig.savefig(f'C:/0.LaLaLand/0.Phystech/2kurs/task1_{name1[i]}.png')
        plt.show()

def task_2():
    out = open("C:/0.LaLaLand/0.Phystech/2kurs/out2.txt", 'r')

    num = np.array(list(map(float, out.readline().strip().split(' '))))

    # prob = [1/100, 11/100, 10/100]

    # name = ['Discrete{1}', 'Segment{0, 10}', 'Set{1, 3, 5, 7, 23, 48, 57, 60, 90, 99}']
    name = ['Segment', 'Set']

    for i in range(2):
        tmp = np.array(list(map(float, out.readline().strip().split(' '))))
        fig, ax = plt.subplots()
        ax.grid(True)
        # ax.set(xlabel='N', ylabel='t',
        #        title='Полная энергия системы')
        ax.set(ylabel='$p$', xlabel='n',
            title=f'{name[i]} от кол-ва эллементов')
        ax.set_xlim(0, 100)
        ax.plot(num, tmp, label='$p_{exp}(n)$')
        ax.scatter(num, tmp)
        ax.plot(num, num/100, label='$p_{teor}(n)$')
        ax.scatter(num, num/100)
        plt.legend()
        fig.savefig(f'C:/0.LaLaLand/0.Phystech/2kurs/task2_{name[i]}.png')
        plt.show()






if __name__ == "__main__":
    # task_1()
    task_2()