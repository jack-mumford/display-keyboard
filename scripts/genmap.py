
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

print('enum EventID {')
idx = 0
for var in vars:
    if type(var) is tuple:
        print("  %s = %d, // %s" % (var[0], idx, var[1]))
    else:
        print("  %s = %d," % (var, idx))
    idx += 1
print('};')

print('switch (event_id) {')
idx = 0
for var in vars:
    if type(var) is tuple:
        print("    case %d: return \"%s\";" % (idx, var[0]))
    else:
        print("    case %d: return \"%s\";" % (idx, var))
    idx += 1
print('}')

key_map = {
    'R0_C3': 'HID_KEY_SHIFT_LEFT',
    'R0_C2': 'HID_KEY_Q',
    'R0_C1': 'HID_KEY_W',
    'R0_C0': 'HID_KEY_E',
    'R1_C3': 'HID_KEY_CONTROL_LEFT',
    'R1_C2': 'HID_KEY_A',
    'R1_C1': 'HID_KEY_S',
    'R1_Co': 'HID_KEY_D',
}
print()
print()
print('constexpr std::array<uint8_t, %d> kEventID2KeyCode = {' % len(vars))
event_id = 0
for v in vars:
    if type(v) is tuple:
        var = v[0]
    else:
        var = v
    key_code = 0
    if var in key_map:
        key_code = key_map[var]
    else:
        key_code = 'HID_KEY_NONE'
    print("  %s, // %d: %s" % (key_code, event_id, var))
    event_id += 1
print('};')