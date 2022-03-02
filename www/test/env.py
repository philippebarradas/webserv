import sys, os

print("Content-Type: text/html\r\n")

for name, value in os.environ.items():
	print("%s\t= %s <br/>" % (name, value))
