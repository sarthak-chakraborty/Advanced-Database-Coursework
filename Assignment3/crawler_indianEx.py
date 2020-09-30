import requests,os,shutil
from bs4 import BeautifulSoup
import pandas as pd

# Only change this parameter if more files are required
documents_required = 500

# Open existing processed files from csv
f = open("scrapped/info.csv", "a")
# Start URL
processed_link = {}
df = pd.read_csv('scrapped/info.csv')
for d in range(len(df)):
	processed_link[df.iloc[d]['URL']] = 1
count = len(processed_link)
URLS = [
'https://indianexpress.com/section/india/page/1/',
'https://indianexpress.com/section/india/page/2/',
'https://indianexpress.com/section/india/page/3/',
'https://indianexpress.com/section/india/page/4/',
'https://indianexpress.com/section/india/page/5/',
'https://indianexpress.com/section/india/page/6/',
'https://indianexpress.com/section/india/page/7/',
'https://indianexpress.com/section/india/page/8/',
'https://indianexpress.com/section/india/page/9/',
'https://indianexpress.com/section/india/page/10/',
'https://indianexpress.com/section/india/page/11/',
'https://indianexpress.com/section/india/page/12/',
'https://indianexpress.com/section/india/page/13/',
'https://indianexpress.com/section/india/page/14/',
]

for u in URLS[:14]:
	page = requests.get(u)
	soup = BeautifulSoup(page.content, 'html.parser')
	links = soup.find_all('a', href=True)
	for l in links:
		l = l['href']
		try:
			if l in processed_link:
				continue
			if len(l) >= 60 and l[:26] =="https://indianexpress.com/":
				URLS.append(l)
		except:
			pass
print("Total Initial URLS: ",len(URLS))
print("url 14 is ",URLS[13])
print("url 15 is ",URLS[14])
print("url 16 is ",URLS[15])
print("url 400 is ",URLS[399])
print("url 500 is ",URLS[499])


for u in URLS:
	if u in processed_link:
		continue
	try:
		print("trying url ",u)
		page = requests.get(u)
		soup = BeautifulSoup(page.content, 'html.parser')
		links = soup.find_all('a', href=True)
		if u not in processed_link:
			print("trying url ",u," not in processed link. So its cool.")
			results = soup.find(id="section")
			images = results.find_all('img')
			print("Processing Document: ",count)
			# Process only those place which has a news body
			if results!=None:
				# print("Processing Document: ",count)
				print("tried url ",u," has results")
				count = count + 1
				job_elems = results.find_all('p')
				print("[[[mining results ", job_elems[0].text)

				f.write("data/"+str(count)+",")
				os.makedirs("scrapped/data/"+str(count))
				os.makedirs("scrapped/data/"+str(count)+"/images")
				textfile = open("scrapped/data/"+str(count)+"/content.txt",'w')
				print("...atleast we are here")
				# Write content to file
				for job_elem in job_elems:
					print("##writing to file")
					try:
						print("###writing to file")
						textfile.write(job_elem.text+"\n")
						print(job_elem.text,"\n")
					except:
						pass
				# Process images
				imagecount = 0
				print("&&&& just above images")
				for im in images:
					try:
						print("***in images")
						##imgsrc = im.get('data-src')
						imgsrc=im['src']
						if imgsrc is not None:
							print("$$$ link found")
							extension = imgsrc.split('.')[-1]
							if extension=='jpg' or extension=='png':
								response = requests.get(imgsrc)
								file = open("scrapped/data/"+str(count)+"/images/image"+str(imagecount)+"."+extension, "wb")
								file.write(response.content)
								file.close()
								imagecount = imagecount + 1
					except:
						pass
				f.write(str(imagecount)+","+u+"\n")
				textfile.close()
				processed_link[u]=1
		if count>=documents_required:
			break
		# Get all links from the page
		for l in links:
			l = l['href']
			try:
				if l in processed_link:
					continue
				if len(l) >= 60 and l[:26] =="https://indianexpress.com/":
					URLS.append(l)
			except:
				pass
	except Exception as e:
		pass
f.close()
