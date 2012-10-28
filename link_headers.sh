rm -rf "./include"

lf_directory="./include"
mkdir -p "$lf_directory"
srcdir="."

# Link them
lf_subdirs="src/WCppUtils		\
					src/EventSystem		\
					src/Widget			\
					src/Window			\
					src/CommonControls	\
					src/GeneralControls"
for lf_dir in $lf_subdirs
do
  # Otherwise go ahead and link
  echo "linking headers from $srcdir/$lf_dir"
  # Check if the Headers file exists
  if test -f "$srcdir/$lf_dir/Headers"
  then
    for lf_file in `(cd $srcdir/$lf_dir; cat Headers)`
    do
      rm -f $lf_directory/$(basename "$lf_file")
      cp "$srcdir/$lf_dir/$lf_file" "$lf_directory/$(basename "$lf_file")"
    done
  else
    echo "Warning: No Headers file for $srcdir/$lf_dir"
  fi
done