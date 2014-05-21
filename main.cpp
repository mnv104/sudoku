#include <stdlib.h>
#include "sset.h"

int main(int argc, char *argv[])
{
	char c, *fname = NULL;
	matrix m(9, 9);

	while ((c = getopt(argc, argv, "f:")) != EOF)
	{
		switch (c) {
			case 'f':
				fname = optarg;
				break;
			case '?':
			case 'h':
				cerr << "Usage: " << argv[0] << " -f <filename> " << endl;
				exit(1);
		}
	}
	if (fname)
		m.input(fname);
	else 
		m.input("wed.txt");

	m.solve();
	m.show();
	return  0;
}
