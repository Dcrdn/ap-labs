def ls_recursivo(string, sofar, pos):
    if pos==-1:
        return 0
    elif string[pos] in sofar:
        return 0
    else:
        helper=sofar+string[pos]
        return max(ls_recursivo(string,helper,pos-1)+1, ls_recursivo(string,sofar,pos-1))

#word="abcdafg"
print("Tell me de string: ")
word=str(input())
print(ls_recursivo(word,"",len(word)-1))