#!sh

DEV="inet"
PATHS=$(grep -v 0.0.0.0 results | cut -f1)
echo "PATHS: $PATHS"
for CONN in /peek/$DEV/tcp_fd_table/*
do
#echo "CONN: $CONN"
LINK=$(readlink $CONN/tf_conn)
#echo "LINK: $LINK"
NUM=$(echo $LINK | xargs -n 1 basename)
#echo "NUM: $NUM"
if [ "$NUM" != "null" ]; then
	echo "$CONN -> $LINK"
fi 

done

