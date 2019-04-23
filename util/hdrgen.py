"""
Generate headers, e.g.

/*******************************************************************************
 *
 *                      R U N T I M E   T A B L E S
 *
 ******************************************************************************/


Usage: python hdrgen.py <allo the header>

"""

import sys

t = ' '.join(sys.argv[1:])

if len(t) < 40:
	t = ' '.join(list(t))

print("/" + "*"*79)
print(" *" + " "*77 + "*" )
print(" *" + t.center(76) + " *")
print(" *" + " "*77 + "*" )
print(" *" + "*"*77 + "/")
