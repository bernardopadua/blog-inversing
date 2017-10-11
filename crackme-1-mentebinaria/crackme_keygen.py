print("####### KEYGEN CRACKME - MENTEBINARIA ########")
print("|    User: Length(9)  Pass: Length(10)       |")
print("##############################################\n\n")

user = input("Username: ")
pasw = ""

key  = len(user)-1

for c in user:
	pasw += chr(ord(c)+key)
	key -= 1

pasw += pasw[0]

print("Password: " + pasw)