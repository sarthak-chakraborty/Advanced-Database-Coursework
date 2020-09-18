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
'https://www.ndtv.com/india/page-1',
'https://www.ndtv.com/india/page-2',
'https://www.ndtv.com/india/page-3',
'https://www.ndtv.com/india/page-4',
'https://www.ndtv.com/india/page-5',
'https://www.ndtv.com/india/page-6',
'https://www.ndtv.com/india/page-7',
'https://www.ndtv.com/india/page-8',
'https://www.ndtv.com/india/page-9',
'https://www.ndtv.com/india/page-10',
'https://www.ndtv.com/india/page-11',
'https://www.ndtv.com/india/page-12',
'https://www.ndtv.com/india/page-13',
'https://www.ndtv.com/india/page-14',
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
			if len(l) >= 60 and l[:21] =="https://www.ndtv.com/":
				URLS.append(l)
		except:
			pass
print("Total Initial URLS: ",len(URLS))

for u in URLS:
	if u in processed_link:
		continue
	try:
		page = requests.get(u)
		soup = BeautifulSoup(page.content, 'html.parser')
		links = soup.find_all('a', href=True)
		if u not in processed_link:

			results = soup.find(id='ins_storybody')
			images = results.find_all('img')
			print("Processing Document: ",count)
			# Process only those place which has a news body
			if results!=None:
				# print("Processing Document: ",count)
				count = count + 1
				job_elems = results.find_all('p')
				f.write("data/"+str(count)+",")
				os.mkdir("scrapped/data/"+str(count))
				os.mkdir("scrapped/data/"+str(count)+"/images")
				textfile = open("scrapped/data/"+str(count)+"/content.txt",'w')
				# Write content to file
				for job_elem in job_elems:
					try:
						textfile.write(job_elem.text+"\n")
					except:
						pass
				# Process images
				imagecount = 0
				for im in images:
					try:
						imgsrc = im.get('data-src')
						if imgsrc is not None:
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
				if len(l) >= 60 and l[:21] =="https://www.ndtv.com/":
					URLS.append(l)
			except:
				pass
	except Exception as e:
		pass
f.close()
