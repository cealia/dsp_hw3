import sys

with open(sys.argv[1], encoding='big5-hkscs', mode='r') as f: #'Big5-ZhuYin.map'
    lines = f.readlines()
zdic = {}
d = {}
for l in lines:
    zs = list(set([i[0] for i in l.strip('\n').split(' ')[1].split('/')]))
    b5 = l.strip('\n').split(' ')[0]
    d[b5] = b5
    for z in zs:
        if z not in zdic:
            zdic[z] = []
        zdic[z].append(b5)
with open(sys.argv[2], encoding='big5-hkscs', mode='w') as f: #'ZhuYin-Big5.map'
    for k,v in zdic.items():
        f.write(k+' ')
        f.write(' '.join(v))
        f.write('\n')
    for k,v in d.items():
        f.write(k+' ')
        f.write(' '.join(v))
        f.write('\n')
        