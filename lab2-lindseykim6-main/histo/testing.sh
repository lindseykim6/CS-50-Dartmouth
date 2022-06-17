#!/bin/bash
cat > foo <<EOF
3 -4 5 1 7 0
8 0 15 12 3 5
1 2 2 2 1 3
EOF

# test case 1
./histo < foo

cat > foo2 <<EOF
3 1 2 5 3 3 3 3
17
32
63
5
10
1
-7
EOF

# test case 2 (expands bins one at a time)
./histo < foo2

cat > foo3 <<EOF
200
10
1
EOF

# test case 3 (expands bins all at once)
./histo < foo3

# test case 4 (given)
echo {1..16} 150 | ./histo
