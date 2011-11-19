# ! /usr/bin/bash
#

echo "Compiling huff.c:"
gcc -Werror -Wall -O3 huff.c -o huff
if (( $? != 0 )) 
then
    echo "    Errors were found when compiling huff.c"
    exit 1
fi
echo "    huff.c compiled without errors."
echo
echo "Compiling unhuff.c:"
gcc -Werror -Wall -O3 unhuff.c -o unhuff
if (( $? != 0 )) 
then
    echo "    Errors were found when compiling unhuff.c"
    exit 1
fi
echo "    unhuff.c compiled without errors."
echo
echo "Running ./huff Samples/triv.txt"
./huff Samples/triv.txt > hout
echo
echo "Running ./unhuff Samples/triv.txt.huff"
./unhuff Samples/triv.txt.huff > uout
echo
echo "diff hout uout:"
diff -a hout uout
echo
echo "diff Samples/triv.txt Samples/triv.txt.huff.unhuff"
diff -a Samples/triv.txt Samples/triv.txt.huff.unhuff
