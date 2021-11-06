import sqlite3
conn = sqlite3.connect('cs_course_scheduling.db')
print("Opened database successfully")

year = input("Enter the academic year: ")
cursor = conn.execute("SELECT first_name, last_name, academic_year FROM students WHERE academic_year=? ORDER BY last_name ASC", [year])
for row in cursor:
	print("First Name    = ", row[0])
	print("Last Name     = ", row[1])
	print("Academic Year = ", row[2])
	print("----------------------------------------")

print("Operation done successfully")
conn.close()

