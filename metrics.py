import os, sys

def endswith(string, filters):
    for f in filters:
        if string.endswith(f):
            return True
    return False

path = sys.argv[1]

file_filter = [ff.strip(" \"") for ff in sys.argv[2].split(",")] # java|glsl|fs|vs
if len(sys.argv) > 3:
    ignore = [ff.strip(" \"") for ff in sys.argv[3].split(",")]
else:
    ignore = []
files_filtered = []

for root, dirs, files in os.walk(path):
    for f in files:
        if endswith(f, file_filter) and f not in ignore:
            files_filtered.append((f, os.path.join(root, f)))

metrics = 0
mdata = []
for fn, filename in files_filtered:
    with open(filename, "r") as f:
        lines = f.readlines()
        metrics_this_file = 0
        for l in lines:
            if not l.strip("\t ").startswith("//") and len(l.strip("\t")) > 0:
                metrics_this_file += 1
	
        mdata.append((metrics_this_file, fn))
        metrics += metrics_this_file

mdata.sort(key=lambda x: x[0])
for num, file in mdata:
    fn_fmt = file.rjust(32)
    num_fmt = str(num).rjust(5)
    ln_fmt = "%s %s of code." % (num_fmt, "line" if num == 1 else "lines")
    
    print("%s : %s" % (fn_fmt, ln_fmt))

msg = "This project has %d %s of code." % (metrics, "line" if metrics == 1 else "lines")
print("------------------------------------------------------------------------------")
print(msg)
