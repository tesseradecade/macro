while read -r line
do
    echo "$line"
done <<< $(python -c "
import site;
import os; import shutil;
import re;
pattern = re.compile(r'(^macro$)|(^cmacro\.cpython-.*?\.so$)|(^macro-.*?\.egg-info$)');
site_pkg = site.getsitepackages()[0] + os.sep;
for pkg in os.listdir(site_pkg):
  if re.match(pattern, pkg):
    if os.path.isfile(site_pkg + pkg):
      os.remove(site_pkg + pkg);
    else:
      shutil.rmtree(site_pkg + pkg);
")
echo "Uninstalled!"