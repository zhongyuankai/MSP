echo "Removing extra files..."
find . -name "doc" -print0 | xargs -0 -- rm -rf
find . -name "*.htm*" -delete
find . -name "*.png" -delete
find . -name "*.bmp" -delete
find . -name "*.jpg" -delete

rm -rf doc
rm -rf tools
find . -name '*.html' | xargs rm
find . -name '*.htm' | xargs rm
find . -name '*.jam' | xargs rm
find . -name '*.xml' | xargs rm
find . -type d -and -name 'doc' | xargs rm -rf
find . -type d -and -name 'example' | xargs rm -rf
find . -type d -and -name 'meta' | xargs rm -rf
find . -name 'sublibs' | xargs rm
find . -type d -and -name 'more' | xargs rm -rf
find . -type d -and -name 'status' | xargs rm -rf
