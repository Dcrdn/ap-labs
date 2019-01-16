def ls(string):
    seen=set()
    size=0
    local=0
    for pos in range(0,len(string)):
        if string[pos] in seen:
            seen=set(string[pos])
            local=1
        else:
            seen.add(string[pos])
            local+=1
        if(local>size):
            size=local
    return size

word="bbbbb"
print(ls(word))