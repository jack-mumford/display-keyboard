
vars = ['NONE']
for r in range(8):
    for c in range(11):
        vars.append("R%d_C%d" % (r, c))

for i in range(8):
    vars.append("R%d_GND" % i)
gpi = 1
for r in range(8):
    vars.append(("GPI%d" % gpi, "(ROW %d)" % r))
    gpi += 1
for c in range(11):
    vars.append(("GPI%d" % gpi, "(COL %d)" % c))
    gpi += 1

vars.extend(['LOGIC_1', 'LOGIC_2'])
vars.extend(['UNUSED_%d' % i for i in range(9)])
vars.append('UNLOCK_WILDCARD')

idx = 0
for var in vars:
    if type(var) is tuple:
        print("%s = %d, // %s" % (var[0], idx, var[1]))
    else:
        print("%s = %d," % (var, idx))
    idx += 1
