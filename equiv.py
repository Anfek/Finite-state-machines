import sys
import array
import copy
import re
import math

def ft_clear(arr):
    line = []
    for i in arr:
        if i != ' ' and i != '\n':
            line.append(i)
    return line;


def ft_atoi(str):
    neg = 1
    if (str[0] == '-'):
        neg = -1
    ans = 0
    i = 0
    if (neg == -1):
        i = i + 1
   # print(len(str))
    while (i < len(str)):
    #    print(str[i])
        cur = (int)(str[i])
        ans = ans * 10 + cur
        i = i + 1
    ans = ans * neg
    return ans;

def ft_new_state(num, fi, inp, p):
   # print("Before Atoi", num)
    bin_str = "{0:0{p}b}".format(num, p = p)
   # bin_str = f'{num:0{p}b}'
   # print("BIN", bin_str)
    shift = bin_str[1:] + fi[num * 2 + inp]
   # print("SHIFT", shift)
    a = int(shift, 2)
    return str(a)

def ft_first_arr(output, num, fi):
    array = []
    for i in range(4):
        array.append([])
    #print(output)
   # x = int(math.log(len(fi)/2, 2))
   # output[i *2 + fi[num * 2 + 0]]
   # n = ft_
    for i in range(pow(2, ft_atoi(num))):
        #print(i)
        if output[i * 2 + int(fi[i * 2 + 0])] == '0' and output[i * 2 + int(fi[i * 2 + 1])] == '0':
            array[0].append(i)
        if output[i * 2 + int(fi[i * 2 + 0])] == '0' and output[i * 2 + int(fi[i * 2 + 1])] == '1':
            array[1].append(i)
        if output[i * 2 + int(fi[i * 2 + 0])] == '1' and output[i * 2 + int(fi[i * 2 + 1])] == '0':
            array[2].append(i)
        if output[i * 2 + int(fi[i * 2 + 0])] == '1' and output[i * 2 + int(fi[i * 2 + 1])] == '1':
            array[3].append(i)
    i = 0
    n = 4
    while i < n:
        if (len(array[i]) == 0):
            del array[i]
            i -= 1
            n -= 1
        i += 1
   # print("ARRAY_1", array[0])
   # print("ARRAY_2", array[1])
   # print("ARRAY_3", array[2])
   # print("ARRAY_4", array[3])
    return array;

def ft_in_one_class(a, b, prev_eq, fi):
    x = int(math.log(len(fi)/2, 2))
   # print("STEPEN", x)
   # print("Is in one class", a, b)
    c = int(ft_new_state(a, fi, 0, x))
    d = int(ft_new_state(b, fi, 0, x))
    e = int(ft_new_state(a, fi, 1, x))
    f = int(ft_new_state(b, fi, 1, x))
   # print("STATE", c, e, d, f)
    for k in prev_eq:
        if a in k:
            if b in k:
                for i in prev_eq:
        #print(i, c);
        #print(0)
                     if c in i:
          #  print("!")
                        if d in i:
           #     print(2)
                            for j in prev_eq:
                                if e in j:
            #            print(3)
                                    if f in j:
             #               print(4)
                                        return True;
                                    else:
              #              print("E is not in j")
                                        return False;
                        else:
               # print("D is not in i")
                            return False;
            else:
                return False;

def ft_find_class(simb, new_eq, ind, prev_eq, fi):
    for i in reversed(new_eq):
        if i == new_eq[ind]:
            break
   #     print("WHY", i, simb, ind)
        if (ft_in_one_class(simb, i[0], prev_eq, fi)):
            i.append(simb)
            return;
    new_eq.append([simb])


def ft_array(prev_eq, fi):
    new_eq = copy.deepcopy(prev_eq)
    #print("PREV_EQ", prev_eq)
    for i in range(0, len(prev_eq)):
        for i2 in range(1, len(prev_eq[i])):
        #    print(prev_eq)
         #   print("FIRST", prev_eq[i][0], prev_eq[i][i2])
            if (not ft_in_one_class(prev_eq[i][0], prev_eq[i][i2], prev_eq, fi)):
       #         print("They are not in the same class")
                new_eq[i].remove(prev_eq[i][i2])
                ft_find_class(prev_eq[i][i2], new_eq, i, prev_eq, fi)
      #          print("NEW_EQ", new_eq, "\n\n\n")
       #     else:
     #           print("Yes, they are in one class")
    #            print("NEW_EQ", new_eq, "\n\n\n")
    return new_eq
                #если не вместе то проверить с первыми элементами массивов, которые тоже были в классе эквивалентности
             #если таких нет - создать новый класс
           # for i3 in range(i2 + 1, len(mas[i])):
            #    if (!ft_in_one_class(mas[i][i2], mas[i][i3])):
             #       mas[i][i3])
              #      new_eq[i].remove(


if __name__ == "__main__":
    f = open(sys.argv[1], 'r')
    fd = f.readlines()
    fd_0 = ft_clear(fd[0])
    fd_1 = ft_clear(fd[2])
    fd_2 = ft_clear(fd[4])
    counter = 0
    #print(fd_0)
    #print(fd_1)
    #print(fd_2)
    prev_eq = ft_first_arr(fd_2, fd_0, fd_1)
    counter = counter + 1
    print("~%d:" % (counter), prev_eq)
    if len(prev_eq) != 1: 
        new_eq = ft_array(prev_eq, fd_1)
        counter = counter + 1
   # print("\n\n\nNEW ITERATION\n\n\n")
    while (prev_eq != new_eq):
        print("\n~%d:" % (counter), new_eq)
        prev_eq = new_eq
        new_eq = ft_array(prev_eq, fd_1)
     #   print("\n\n\nNEW ITERATION\n\n")
        if (prev_eq != new_eq):
            counter = counter + 1
    print("\nFinal is ~%d:" % (counter), new_eq)
    print("Miu is", counter)
    print("Sigma is", len(new_eq))
    if (len(new_eq) == pow(2, ft_atoi(fd_0))):
            print("It's minimal")
    else:
        print("It's not minimal")
        

f.close()
