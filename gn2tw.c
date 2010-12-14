#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "sak/sakcore.h"

#define PROG_NAME "gn2tw"
#define PROG_VERSION "1.0"

/** Google Notebook's atom format.
 * <feed> root, which has a <title> which is the notebook's name.
 * <entry> element. There are three types of entries. 1. Notebook itself, with empty title. 2. Section, with empty content. 3. Note.
 * <category scheme='http://schemas.google.com/notebook/gdata/2007/section' term='SDRhiDAoQkIeBy9Aj' label=''/> `term' is the entry's section name.
 * <category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/notebook/2008/kind#note'/> it's only used by section entry.
 * <content>
 */

/** TiddlyWiki's format
 * All tw, except for script tw, are included between
 * <!--POST-SHADOWAREA--><div id="storeArea">
 * and
 * </div><!--POST-STOREAREA-->
 * A tw is defined as
 * <div title="A tw title" modifier="ytxie" created="201011272310" modified="201011282310" tags="ATag" changecount=""><pre>something</pre></div>
 * where `modified' is not neccessory and `changecount' is counted from 1. 
 */

typedef struct Tag {
	xmlChar		id[32];
	xmlChar		title[256];
} Tag, *TagPtr;

extern char* g_twHtmlTxt;
extern const long g_twHtmlTxtLen;
extern char* g_twTemplate;

xmlNodePtr g_wikiStore = knull;
xmlChar* g_authorName = "ytxie";
Tag	g_topTag;
SakListPtr g_tagList = knull;

xmlChar*
findTag(
	xmlChar*	term)
{
	xmlChar* re = knull;
	SakListPtr pList = g_tagList;

	SAK_FUNCNAME("findTag");

	__SAK_BEGIN__;

	SAK_ASSERT(term);

	if (xmlStrcmp(g_topTag.id, term) == 0) {
		re = g_topTag.title;
	}
	else {
		while (pList) {
			TagPtr p = (TagPtr) pList->first;
			if (xmlStrcmp(p->id, term) == 0) {
				re = p->title;
				break;
			}
			pList = pList->rest;
		}
	}

	__SAK_END__;

	return re;
}

void guessTitle(xmlChar* title)
{
	kbool hasChinese = kfalse;
	kbool hasEnglish = kfalse;
	kbool isEnglish = kfalse;
	const int maxLen = 48;
	xmlChar* p = title;
	int n = 1;
	int nChinese = 0;
	int nEnglish = 0;

	if (xmlStrncmp(title, (xmlChar*) "write a transform", 17) == 0)
		nEnglish = 1;

	while (*p != '\0' && n < maxLen) {
		if (*p >= 127) {
			if (!hasChinese)
				hasChinese = ktrue;
			nChinese++;
		}
		if (isalpha(*p)) {
			//nEnglish++;
			if (!hasEnglish)
				hasEnglish = ktrue;
			p++;
			if (*p >= 127)
				p--;
			else {
				kbool hasSpace = kfalse;
				while (*p < 127) {	/* DEL is 127 */
					while (isspace(*p)) {
						if (!hasSpace)
							hasSpace = ktrue;
						p++;
					}
					if (hasSpace && (isalpha(*p) || *p > 127)) {
						nEnglish++;
						isEnglish = ktrue;
						break;
					}
					p++;
				}
			}
		}

		p++;
		n++;
	}

	p = title;
	if (!isEnglish) {
		while (*p != '\0') {
			if (isspace(*p) || *p == '\n' || *p == '\r') {
				*p = '\0';
				break;
			}
			p++;
		}
	}
	else if (!hasChinese) {
		kbool toStop = kfalse;
		n = 1;
		while (*p != '\0') {
			if ((*p == '.' || *p == '?') && isspace(*(p+1))) {
				if (*p == '.')
					*p = '\0';
				else 
					*(p+1) = '\0';
				break;
			}
			if (!toStop && n >= maxLen) {
				toStop = ktrue;
			}
			if (toStop) {
				if (isspace(*p) || *p == '.' || *p == '?' ||
					*p == ';' || *p == '!') {
					*p = '\0';
					break;
				}
			}
			p++;
			n++;
		}
	}
	else {
		int n = nEnglish/4;
		while (*p != '\0') {
			if (isspace(*p)) {
				if (n)
					n--;
				else {
					*p = '\0';
					break;
				}
			}
			p++;
		}
	}
}

kbool
addTW(xmlChar* title, xmlChar* time, xmlChar* term, xmlChar* label, xmlChar* content)
{
	xmlNodePtr div;
	xmlNodePtr pre;
	xmlAttrPtr titleProp, modifierProp, createdProp, changecountProp, tagsProp;
	xmlChar *p = knull;
	xmlChar* sectionTitle = knull;
	xmlChar *tag = knull;

	SAK_FUNCNAME("addTW");

	__SAK_BEGIN__;

	/* create tags */
	sectionTitle = findTag(term);
	{
		int n = xmlStrlen(g_topTag.title) + xmlStrlen(sectionTitle) + xmlStrlen(label) + 3;
		xmlChar* p[3] = {g_topTag.title, sectionTitle, label};
		int i;
		SAK_CALL(tag = sakAlloc(n * sizeof(xmlChar)));
		if (xmlStrcmp(g_topTag.title, sectionTitle) == 0)
			p[1] = knull;
		if (sectionTitle && label && xmlStrcmp(sectionTitle, label) == 0)
			p[2] = knull;
		if (label && xmlStrcmp(g_topTag.title, label) == 0)
			p[2] = knull;
		sprintf(tag, "%s", p[0]);
		for (i = 1; i < 3; i++) {
			if (p[i])
				sprintf(tag, "%s %s", tag, p[i]);
		}
	}

	/* create title */
	guessTitle(title);

	SAK_STD_CALL(div = xmlNewTextChild(g_wikiStore, knull, "div", knull));
	SAK_STD_CALL(titleProp = xmlNewProp(div, "title", title));
	SAK_STD_CALL(modifierProp = xmlNewProp(div, "modifier", g_authorName));
	SAK_STD_CALL(createdProp = xmlNewProp(div, "created", time));
	SAK_STD_CALL(tagsProp = xmlNewProp(div, "tags", tag)); 
	SAK_STD_CALL(changecountProp = xmlNewProp(div, "changecount", (const xmlChar*) "1"));
	SAK_STD_CALL(pre = xmlNewTextChild(div, knull, "pre", content));

	__SAK_END__;

	if (tag)
		sakFree(tag);

	if (SAK_SUCCESSED(sakGetErrStatus()))
		return ksuccess;
	else
		return kfail;
}

static char* compactStr(char* str)
{
	char* p = strchr(str, ' ');
	char* pre = p;
	int n = 0;

	while (p) {
		while (*(p+1) == ' ')
			p++;
		*(p+1) = toupper(*(p+1));
		n = strlen(str) - (p - str);	/* include '\0' */
		memmove(pre, p+1, n);
		p = strchr(str, ' ');
		pre = p;
	}
}

kbool
addTag(xmlChar* id, xmlChar* title, int level)
{
	SAK_FUNCNAME("addTag");

	__SAK_BEGIN__;

	compactStr(title);

	if (level == 0) {
		strcpy(g_topTag.id, id);
		strcpy(g_topTag.title, title);
	}
	else {
		TagPtr tag = knull;
		SAK_CALL(tag = (TagPtr) sakAlloc(sizeof(*tag)));
		strcpy(tag->id, id);
		strcpy(tag->title, title);
		SAK_CALL(g_tagList = sakListPush(g_tagList, (void*) tag));
	}

	__SAK_END__;

	if (SAK_SUCCESSED(sakGetErrStatus()))
		return ksuccess;
	else
		return kfail;
}

static xmlNodePtr
parseWiki(xmlDocPtr *pWikiDoc)
{
	xmlDocPtr wikiDoc = knull;
	xmlNodePtr cur = knull;
	xmlNodePtr store = knull;

	SAK_FUNCNAME("parseWiki");
	
	__SAK_BEGIN__;

	SAK_STD_CALL(wikiDoc = xmlParseMemory(g_twTemplate, 29));
	SAK_STD_CALL(cur = xmlDocGetRootElement(wikiDoc));
	SAK_ASSERT(xmlStrcmp(cur->name, (const xmlChar*) "div") == 0);
	store = cur;

	__SAK_END__;

	*pWikiDoc = wikiDoc;
	return store;
}

static kbool
parseEntry (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *id = knull;
	xmlChar time[13];
	xmlChar *title = knull;
	xmlChar *content = knull;
	xmlChar *newContent = knull;
	SakListPtr labelList = knull;
	SakListPtr termList = knull;
	xmlChar *term = knull;
	xmlChar* label = knull;

	SAK_FUNCNAME("parseEntry");

	__SAK_BEGIN__;

	while (cur) {
	    if (xmlStrcmp(cur->name, (const xmlChar *)"id") == 0) {
		    id = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
 	    }
		else if (xmlStrcmp(cur->name, (const xmlChar*) "updated") == 0) {
			xmlChar* updated = knull;
			updated = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			if (updated) {
				xmlChar *year = xmlStrsub(updated, 0, 4);
				xmlChar *month = xmlStrsub(updated, 5, 2);
				xmlChar *day = xmlStrsub(updated, 8, 2);
				xmlChar *hour = xmlStrsub(updated, 11, 2);
				xmlChar *minitue = xmlStrsub(updated, 14, 2);
				xmlStrPrintf(time, 13, "%s%s%s%s%s",
							 year, month, day, hour, minitue);
				xmlFree(updated);
				xmlFree(year);
				xmlFree(month);
				xmlFree(day);
				xmlFree(hour);
				xmlFree(minitue);
			}
		}
		else if (xmlStrcmp(cur->name, (const xmlChar*) "category") == 0) {
			term = xmlGetProp(cur, "term");
			if (term && xmlStrncmp(term, (const xmlChar*) "http://", 7) != 0) {
				xmlChar *e = knull;
				int n = xmlStrlen(term) + 1;
				compactStr(term);
				SAK_CALL(e = (xmlChar*) sakAlloc(n * sizeof(xmlChar)));
				xmlStrPrintf(e, n, "%s", term);
				SAK_CALL(termList = sakListPush(termList, (void*) e));
			}
			label = xmlGetProp(cur, "label");
			if (label && label[0] != '\0') {
					xmlChar *e = knull;
					int n = xmlStrlen(label) + 1;
					compactStr(label);
					SAK_CALL(e = (xmlChar*) sakAlloc(n * sizeof(xmlChar)));
					xmlStrPrintf(e, n, "%s", label);
					SAK_CALL(labelList = sakListPush(labelList, (void*) e));
			}
			if (term) {
				xmlFree(term);
				term = knull;
			}
			if (label) {
				xmlFree(label);
				label = knull;
			}
		}
		else if (xmlStrcmp(cur->name, (const xmlChar*) "title") == 0) 
			title = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		else if (xmlStrcmp(cur->name, (const xmlChar*) "content") == 0) {
			content = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			if (content) {
				int n = xmlStrlen(content);
				SAK_CALL(newContent = (xmlChar*) sakAlloc((n + 256) * sizeof(xmlChar)));
				assert(newContent);
				xmlStrPrintf(newContent, n+255, "<html>%s</html>", content);
			}
		}

		cur = cur->next;
	}

	assert(!term);
	assert(!label);

	if (content) {				/* tw */
		int n = 0;
		xmlChar* term = knull;
		xmlChar* label = knull;
		SakListPtr p;
		p = termList;
		while (termList) {
			xmlChar* e = termList->first;
			if (findTag(e)) {
				termList = sakListPop(termList, (void**)&term);
				SAK_CALL(labelList = sakListAppend(labelList, termList));
				termList = knull;
				break;
			}
			termList = sakListPop(termList, (void**)&term);
			SAK_CALL(labelList = sakListPush(labelList, (void*)term));
			term = knull;
		}
		assert(!termList);
		p = labelList;
		while (p) {
			n += xmlStrlen((xmlChar*)p->first) + 1;
			p = p->rest;
		}
		if (n > 0) {
			SAK_CALL(label = (xmlChar*) sakAlloc(n * sizeof(xmlChar*)));
			p = labelList;
			sprintf(label, "%s", (xmlChar*)p->first);
			p = p->rest;
			while (p) {
				sprintf(label, "%s %s", label, (xmlChar*)p->first);
				p = p->rest;
			}
		}

		SAK_CALL(addTW(title, time, term, label, newContent));

		sakFree(label);
		sakListFree(&labelList);
		sakFree(term);
	}
	else {						/* tag */
		SAK_CALL(addTag(id, title, 1));
	}

	__SAK_END__;
	
	if (id)
		xmlFree(id);
	if (title)
		xmlFree(title);
	if (content)
		xmlFree(content);
	if (newContent)
		sakFree(newContent);
	if (termList)
		sakListFree(&termList);
	if (labelList)
		sakListFree(&labelList);
	if (label)
		xmlFree(label);
	if (term)
		xmlFree(term);

	if (SAK_SUCCESSED(sakGetErrStatus())) 
		return ksuccess;
	else 
		return kfail;
}

static kbool
parseFeed(char *docname)
{
	xmlDocPtr doc = knull;
	xmlNodePtr cur = knull;
	kbool	gotTitle = kfalse;

	SAK_FUNCNAME("parseFeed");

	__SAK_BEGIN__;

	SAK_STD_CALL(doc = xmlParseFile(docname));
	if (doc->encoding && xmlStrcmp(doc->encoding, "UTF-8") != 0 && xmlStrcmp(doc->encoding, "utf-8") != 0) {
		fprintf(stderr, "Warning: Now only UTF-8 encoding is supported.\n");
	}
	SAK_STD_CALL(cur = xmlDocGetRootElement(doc));	
	SAK_ASSERT(xmlStrcmp(cur->name, (const xmlChar *) "feed") == 0);

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if (!gotTitle && (!xmlStrcmp(cur->name, (const xmlChar *)"title"))) {
			xmlChar* title = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			xmlChar* id = knull;
			cur = cur->next;
			while (cur) {
				if (xmlStrcmp(cur->name, (const xmlChar*) "entry") == 0) {
					xmlNodePtr p = cur->xmlChildrenNode;
					while (p) {
						if (xmlStrcmp(p->name, (const xmlChar*) "id") == 0) {
							id = xmlNodeListGetString(doc, p->xmlChildrenNode, 1);
							break;
						}
						p = p->next;
					}
					break;
				}
				cur = cur->next;
			}
			addTag(id, title, 0);
			xmlFree(id);
			xmlFree(title);
			gotTitle = ktrue;
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"entry"))) {
			parseEntry (doc, cur->xmlChildrenNode);
		}
		 
		cur = cur->next;
	}

	__SAK_END__;

	if (g_tagList)
		sakListFree(&g_tagList);
	if (doc)
		xmlFreeDoc(doc);

	if (SAK_SUCCESSED(sakGetErrStatus()))
		return ksuccess;
	else
		return kfail;
}

char* getResultName(char* docname)
{
	char* resultName = knull;
	char* baseName = sakBaseName(docname);
	char* p = strrchr(baseName, '.');
	if (p)
		*p = '\0';
	resultName = (char*) sakAlloc(strlen(baseName) + 6);
	sprintf(resultName, "%s.html", baseName);
	sakFree(baseName);
	return resultName;
}

#define TEMP_NAME "/tmp/tw_temp_201012122233.xml"
		
void saveWiki(char* resultName)
{
	FILE* resultFile = knull;
	FILE* noteFile = knull;
	const int pre = 23997;
	const int midn = 27 + 1;
	const int post = 327947;
	char* buf = knull;
	int noteLen = 0;
	int n = 0;
	char* pWiki = g_twHtmlTxt;
	struct stat noteStat;

	SAK_FUNCNAME("saveWiki");

	__SAK_BEGIN__;

	SAK_STD_CALL(resultFile = fopen(resultName, "w+"));
	SAK_STD_CALL(noteFile = fopen(TEMP_NAME, "r"));

	SAK_CALL(buf = sakAlloc(pre+1));
	fwrite(pWiki, 1, pre, resultFile);
	pWiki += pre + midn;

	n = stat(TEMP_NAME, &noteStat);
	SAK_ASSERT_CODE(n == 0, SAK_StsIO);
	noteLen = noteStat.st_size - 22;
	fseek(noteFile, 22, SEEK_SET);
	if (noteLen <= pre) {
		fread(buf, 1, noteLen, noteFile);
		fwrite(buf, 1, noteLen, resultFile);
	}
	else {
		while (n = fread(buf, 1, pre, noteFile)) {
			fwrite(buf, 1, n, resultFile);
		}
	}

	n = g_twHtmlTxtLen - pre - midn;
	fwrite(pWiki, 1, n, resultFile);

	__SAK_END__;

	if (buf)
		sakFree(buf);
	if (resultFile)
		fclose(resultFile);
	if (noteFile)
		fclose(noteFile);
}

void usage()
{
	printf("%s v%s.\n"
		   "xyt. leeward.xie@gmail.com\n"
		   "Description:\n"
		   "\tConvert a Google Notebooks atom xml file to a TiddlyWiki file.\n"
		   "\tNOTE: The encoding of the atom file should be UTF-8 if you have non-latin characters in the notes.\n"
		   "Syntax:\n"
		   "\t%s [-n <username>] <Google Notebook> [<TiddlyWiki File Name>]\n"
		   "\t%s [-h|-v] Get help or version.\n",
		   PROG_NAME, PROG_VERSION, PROG_NAME, PROG_NAME);
}

int
main(int argc, char **argv)
{
	char *docName = knull;
	xmlDocPtr wikiDoc = knull;
	char *resultName = knull;
	int opt;
	kbool hasSpecifiedName = kfalse;

	while ((opt = getopt(argc, argv, "vhn:")) != -1) {
		switch (opt) {
		case 'h':
			usage();
			exit (0);
			break;
		case 'v':
			fprintf(stdout, "%s: v%s\n", PROG_NAME, PROG_VERSION);
			exit (0);
			break;
		case 'n':
			g_authorName = optarg;
			break;
		default: 
			fprintf(stderr, "Bad args...\n");
			exit(1);
		}
	}

		
	if (optind >= argc) {
		fprintf(stderr, "Error: Expected a Google Notebook file.\n");
		exit(1);
	}

	docName = argv[optind++];
	if (optind < argc) {
		hasSpecifiedName = ktrue;
		resultName = argv[optind];
	}
	else
		resultName = getResultName(docName);

	g_wikiStore = parseWiki(&wikiDoc);
	if (!g_wikiStore) {
		fprintf(stderr, "Error: parse tiddly wiki faield.\n");
		return 1;
	}
	parseFeed (docName);

	xmlSaveFormatFile(TEMP_NAME, wikiDoc, 1);
	xmlFreeDoc(wikiDoc);

	saveWiki(resultName);

	if (!hasSpecifiedName)
		sakFree(resultName);
	remove(TEMP_NAME);

	return 0;
}

