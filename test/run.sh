
if [[ $# -ne 2 ]]
then
    echo Wrong usage!!!
    exit 1
fi

if [[ $1 -gt 1 ]] || [[ $2 -lt 6 ]]
then
    echo Bad parameters
    exit 1
fi

#Change to src
cd ../src/

echo Compiling test source code...
make > /dev/null 2>&1
echo Done

echo Running tests...
# ./TestBellmanFordComplexity $1 $2 > ../test/out.txt
./TestTransitiveClosureComplexity $2 > ../test/out.txt
echo Done

#Comeback from src
cd ../test/

echo Plotting generating figure
python3 plot.py $1
echo Done
