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

for i in small1.txt small2.txt medium1.txt medium2.txt big1.txt big2.txt
do
    echo "Running ./huff Samples/$i"
    time ./huff Samples/$i > hout
    echo
    echo "Running ./unhuff Samples/$i.huff"
    time ./unhuff Samples/$i.huff > uout
    echo
    echo "diff Samples/$i Samples/$i.huff.unhuff"
    diff -a Samples/$i Samples/$i.huff.unhuff
    echo
done




