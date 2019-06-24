echo 'Building Single Linked List Program'
gcc main.c LinkedList.c -Wall
rc=$?
if [ $rc == 0 ]; then
	echo 'Build Complete'
else
	echo 'Build Failed'
fi
