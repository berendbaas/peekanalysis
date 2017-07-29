cd $1
CURR_DIR=$PWD
WORKING_DIR=/root/tmp
echo "current directory: $CURR_DIR"
ls $PWD > $WORKING_DIR/__parent

# Find child directories
CHILD_DIRS=$(find . -maxdepth 1 -type d | tail -n +2)

# Find child links
CHILD_LINKS=$(find . -maxdepth 1 -type l)

# Iterate over directories
for DIR in $CHILD_DIRS; do
	cd $DIR
	ls $(pwd) > $WORKING_DIR/__child
	diff $WORKING_DIR/__parent $WORKING_DIR/__child | wc -l 
	cd $CURR_DIR
done

# Iterate over links
for LINK in $CHILD_LINKS; do
	echo "currently in: $LINK"
	cd $LINK
	ls $(pwd) > $WORKING_DIR/__child
	diff $WORKING_DIR/__parent $WORKING_DIR/__child | wc -l
	cd $CURR_DIR
done
