#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT_LEN 256
#define MAX_OPTION_LEN 128
#define MAX_KNOWLEDGE_LEN 64
#define MAX_EXAM_SIZE 500
#define MAX_LINE_LEN 1024
#define MAX_QUESTIONS_NUMBER 500

typedef struct {
	int id;
	char questionText[MAX_TEXT_LEN];
	char options[4][MAX_OPTION_LEN];
	char correctOption;
	int difficulty;
	char knowledgePoint[MAX_KNOWLEDGE_LEN];
} Question;

typedef struct QuestionNode {
	Question data;
	struct QuestionNode* next;
} QuestionNode;

void replace_space(char *str) {
	for (int i = 0; str[i]; i++) {
		if (str[i] == ' ') str[i] = '_';
	}
}

void clearScreen() {
	// 判断操作系统，Windows 使用 cls 清屏，其他系统使用 clear 清屏
#ifdef _WIN32
	system("cls"); // Windows 系统
#else
	system("clear"); // Linux / macOS 系统
#endif
}

//加载文件进入链表
QuestionNode* load(const char* fname)
{
	
	FILE *fp = fopen(fname, "r");
	if(fp == NULL){
		printf("打开文件：%s 失败\n",fname);
		return NULL;
	}
	
	QuestionNode *head = NULL, *tail = NULL;
	Question new;
	
	while(fscanf(fp,"%d %s %s %s %s %s %c %d %s",&new.id,new.questionText,new.options[0],new.options[1],new.options[2],new.options[3],&new.correctOption,&new.difficulty,new.knowledgePoint)!=EOF)
	{
		QuestionNode *newNode = (QuestionNode*)malloc(sizeof(QuestionNode));
		newNode->data = new;
		newNode->next = NULL;
		if(head == NULL){
			head = newNode;
			tail = newNode;
		}else{
			tail->next = newNode;
			tail = newNode;
		}
		
	}
	
	fclose(fp);
	printf("文件加载完成。\n");
	return head;
}
//释放
void freeList(QuestionNode* L)
{
	QuestionNode* p = L, *q;
	while (p) {
		q = p->next;
		free(p);
		p = q;
	}
}
//录入
void input(char *fname) {
	FILE *fp = fopen(fname, "w");
	if (fp == NULL) {
		printf("打开文件失败！\n");
		exit(0);
	}
	
	int flag = 1;
	Question tmp;
	
	while (flag != 0) {
		printf("请输入录入的题目（序号，题干，所有选项，正确答案，难度，知识点）：\n");
		
		// 读取输入并限制字段长度
		scanf("%d %255s %127s %127s %127s %127s %c %d %63s",
			&tmp.id, tmp.questionText, 
			tmp.options[0], tmp.options[1], tmp.options[2], tmp.options[3],
			&tmp.correctOption, &tmp.difficulty, tmp.knowledgePoint);
		
		// 替换所有字段中的空格为下划线
		replace_space(tmp.questionText);
		for (int i = 0; i < 4; i++) replace_space(tmp.options[i]);
		replace_space(tmp.knowledgePoint);
		
		// 写入文件
		fprintf(fp, "%d %s %s %s %s %s %c %d %s\n",
			tmp.id, tmp.questionText,
			tmp.options[0], tmp.options[1], tmp.options[2], tmp.options[3],
			tmp.correctOption, tmp.difficulty, tmp.knowledgePoint);
		
		printf("请问是否继续录入？是请输入1，否输入0:");
		scanf("%d", &flag);
	}
	
	fclose(fp);
	printf("录入完成！\n");
}

//浏览
void lookThrough(char *fname)
{
	FILE *fp;
	fp = fopen(fname,"r");
	if(fp == NULL )
	{
		printf("文件打开失败！\n");
		exit(0);
	}
	
	Question tmp;
	while(fscanf(fp,"%d %s %s %s %s %s %c %d %s",&tmp.id,tmp.questionText,tmp.options[0],tmp.options[1],tmp.options[2],tmp.options[3],&tmp.correctOption,&tmp.difficulty,tmp.knowledgePoint)!=EOF)
	{
		printf("%d.%s\n",tmp.id,tmp.questionText);
		printf("A.%s\n",tmp.options[0]);
		printf("B.%s\n",tmp.options[1]);
		printf("C.%s\n",tmp.options[2]);
		printf("D.%s\n",tmp.options[3]);
		printf("正确答案：%c   难度：%d\n",tmp.correctOption,tmp.difficulty);
		printf("考察知识点：%s\n",tmp.knowledgePoint);
		printf("\n");
	}
	
	fclose(fp);
	return ;
}

//把链表里的写入文件
int save(char* filename, QuestionNode* head) {
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		printf("无法打开文件：%s\n", filename);
		return 0;
	}
	
	QuestionNode *p = head;
	while (p != NULL) {
//		// 调试输出（可选）
//		printf("DEBUG: %d %s %s %s %s %s %c %d %s\n",
//			p->data.id, p->data.questionText,
//			p->data.options[0], p->data.options[1],
//			p->data.options[2], p->data.options[3],
//			p->data.correctOption, p->data.difficulty,
//			p->data.knowledgePoint);
		
		// 写入文件
		fprintf(file, "%d %s %s %s %s %s %c %d %s\n",
			p->data.id, p->data.questionText,
			p->data.options[0], p->data.options[1],
			p->data.options[2], p->data.options[3],
			p->data.correctOption, p->data.difficulty,
			p->data.knowledgePoint);
		p = p->next;
	}
	
	fclose(file);
	printf("题库已保存到文件：%s\n", filename);
	return 1;
}


//插入
void Insert(char *fname) {
	QuestionNode *head = load(fname);
	Question q;
	
	printf("请输入题目编号，题干，选项，正确答案，难度，知识点：\n");
	scanf("%d %255s %127s %127s %127s %127s %c %d %63s",
		&q.id, q.questionText,
		q.options[0], q.options[1], q.options[2], q.options[3],
		&q.correctOption, &q.difficulty, q.knowledgePoint);
	
	// 替换所有字段中的空格为下划线
	replace_space(q.questionText);
	for (int i = 0; i < 4; i++) replace_space(q.options[i]);
	replace_space(q.knowledgePoint);
	
	// 创建新节点
	QuestionNode *newNode = (QuestionNode*)malloc(sizeof(QuestionNode));
	newNode->data = q;
	newNode->next = NULL;
	
	// 插入到链表头部（当 id=1 时）
	if (q.id == 1) {
		newNode->next = head;
		head = newNode;
	} 
	// 插入到链表中间或尾部
	else {
		QuestionNode *p = head;
		QuestionNode *prev = NULL;
		
		// 遍历链表查找插入位置
		while (p != NULL && p->data.id != q.id) {
			prev = p;
			p = p->next;
		}
		
		if (prev == NULL) {
			// 链表为空，直接插入新节点
			head = newNode;
		} else {
			// 插入到 prev 和 p 之间（或尾部）
			prev->next = newNode;
			newNode->next = p;
		}
	}
	
	// 重新编号并保存
	int cnt = 1;
	for (QuestionNode *p = head; p != NULL; p = p->next) {
		p->data.id = cnt;
		cnt++;
	}
	
	if (save(fname, head)) {
		printf("题目添加成功！\n");
	}
	freeList(head);
}


//追加
void appendQuestion(char *fname)
{
	FILE *fp;
	fp = fopen(fname,"a");
	if(fp == NULL)
	{
		printf("文件:%s打开失败！",fname);
		exit(0);
	}
	
	int flag=1;
	Question tmp;
	
	while(flag != 0)
	{
		printf("请输入追加的题目：\n");
		scanf("%d %s %s %s %s %s %c %d %s",&tmp.id,tmp.questionText,tmp.options[0],tmp.options[1],tmp.options[2],tmp.options[3],&tmp.correctOption,&tmp.difficulty,tmp.knowledgePoint);
		fprintf(fp,"%d %s %s %s %s %s %c %d %s\n",tmp.id,tmp.questionText,tmp.options[0],tmp.options[1],tmp.options[2],tmp.options[3],tmp.correctOption,tmp.difficulty,tmp.knowledgePoint);
		
		printf("请问是否继续追加？是请输入1，否输入0:");
		scanf("%d",&flag);
	}
	
	if(flag==0)
		printf("追加完成！\n");
	
	fclose(fp);
	
	return ;
}

void delete(int id,char* filename) {
	QuestionNode* head = load(filename);
	QuestionNode* curr = head;
	QuestionNode* prev = NULL;
	
	while (curr != NULL) {
		if (curr->data.id == id) {
			if (prev == NULL) {
				head = curr->next;
				free(curr);
				
				int cnt = 1;
				for (QuestionNode *p = head; p != NULL; p = p->next) {
					p->data.id = cnt;
					cnt++;
				}
				
				save(filename,head);
				freeList(head);
				printf("删除成功！\n");
				return ;
			} else {
				prev->next = curr->next;
				free(curr);
				
				int cnt = 1;
				for (QuestionNode *p = head; p != NULL; p = p->next) {
					p->data.id = cnt;
					cnt++;
				}
				
				save(filename,head);
				freeList(head);
				printf("删除成功！\n");
				return ;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	
	printf("未找到编号为 %d 的题目。\n", id);
	freeList(head);
	return ;
}

//查找
void findExact(char *fname,char *knowledgePoint)
{
	FILE *fin;
	FILE *fout;
	fin = fopen(fname,"r");
	fout = fopen("select.txt","w");
	if(fin == NULL||fout == NULL)
	{
		printf("文件：%s打开失败！",fname);
		exit(0);
	}
	
	Question tmp;
	int flag=0;
	
	while(fscanf(fin,"%d %s %s %s %s %s %c %d %s",&tmp.id,tmp.questionText,tmp.options[0],tmp.options[1],tmp.options[2],tmp.options[3],&tmp.correctOption,&tmp.difficulty,tmp.knowledgePoint)!=EOF)
	{
		if(strcmp(tmp.knowledgePoint,knowledgePoint)==0)
		{
			fprintf(fout,"%d %s %s %s %s %s %c %d %s\n",tmp.id,tmp.questionText,tmp.options[0],tmp.options[1],tmp.options[2],tmp.options[3],tmp.correctOption,tmp.difficulty,tmp.knowledgePoint);
			flag = 1;
		}
	}
	
	if(flag == 0)
		printf("题库中不存在符合条件的题目");
	
	fclose(fin);
	fclose(fout);
	return ;
}

QuestionNode* find(QuestionNode* head, int id) {
	QuestionNode* curr = head;
	while (curr != NULL) {
		if (curr->data.id == id) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// 修改题目函数
int modify(char *fname, int id) {
	QuestionNode* head = load(fname);
	QuestionNode *p = find(head, id);
	if (p == NULL) {
		printf("未找到编号为 %d 的题目。\n", id);
		freeList(head);
		return 0;
	}
	
	// 打印原始题目内容
	printf("原题干：%s\n", p->data.questionText);
	printf("新题干：");
	getchar(); // 清除之前输入中的换行符
	fgets(p->data.questionText, MAX_TEXT_LEN, stdin);
	p->data.questionText[strcspn(p->data.questionText, "\n")] = '\0'; // 去除换行符
	
	// 修改选项
	for (int i = 0; i < 4; i++) {
		printf("原选项 %c：%s\n", 'A' + i, p->data.options[i]);
		printf("新选项 %c：", 'A' + i);
		fgets(p->data.options[i], MAX_OPTION_LEN, stdin);
		p->data.options[i][strcspn(p->data.options[i], "\n")] = '\0'; // 去除换行符
	}
	
	// 修改正确答案
	printf("原正确答案：%c\n", p->data.correctOption);
	printf("新正确答案（A~D）：");
	scanf(" %c", &p->data.correctOption); // 注意空格跳过换行符
	getchar(); // 清除输入缓冲区中的换行符
	
	// 修改难度
	printf("原难度：%d\n", p->data.difficulty);
	printf("新难度（1-5）：");
	scanf("%d", &p->data.difficulty);
	getchar(); // 清除输入缓冲区中的换行符
	
	// 修改知识点
	printf("原知识点：%s\n", p->data.knowledgePoint);
	printf("新知识点（无空格）：");
	scanf("%63s", p->data.knowledgePoint); // 限制输入长度
	getchar(); // 清除输入缓冲区中的换行符
	
	printf("修改成功！\n");
	
	// 保存修改后的数据
	save(fname, head);
	freeList(head);
	return 0;
}



//按难度排序
void order(int num) {
	FILE* fp = fopen("select.txt", "r");
	if (!fp) {
		printf("文件打开失败！\n");
		return;
	}
	
	Question* tmp = (Question*)malloc(MAX_EXAM_SIZE * sizeof(Question));
	if (!tmp) {
		fclose(fp);
		printf("内存分配失败！\n");
		return;
	}
	
	int i = 0;
	while (i < MAX_EXAM_SIZE && fscanf(fp, "%d %s %s %s %s %s %c %d %s",
		&tmp[i].id, tmp[i].questionText,
		tmp[i].options[0], tmp[i].options[1],
		tmp[i].options[2], tmp[i].options[3],
		&tmp[i].correctOption, &tmp[i].difficulty,
		tmp[i].knowledgePoint) != EOF) {
		i++;
	}
	fclose(fp);
	
	// 冒泡排序
	for (int j = 0; j < i - 1; j++) {
		for (int k = 0; k < i - j - 1; k++) {
			if (tmp[k].difficulty > tmp[k + 1].difficulty) {
				Question swap = tmp[k];
				tmp[k] = tmp[k + 1];
				tmp[k + 1] = swap;
			}
		}
	}
	
	fp = fopen("select.txt", "w");
	if (!fp) {
		free(tmp);
		printf("文件写入失败！\n");
		return;
	}
	
	for (int j = 0; j < i; j++) {
		fprintf(fp, "%d %s %s %s %s %s %c %d %s\n",
			tmp[j].id, tmp[j].questionText,
			tmp[j].options[0], tmp[j].options[1],
			tmp[j].options[2], tmp[j].options[3],
			tmp[j].correctOption, tmp[j].difficulty,
			tmp[j].knowledgePoint);
	}
	
	fclose(fp);
	free(tmp);
}


//组卷
void generate(char *fname, char* knowledgePoint, int numQuestions) {
	// 第一步：筛选符合条件的题目并写入临时文件
	findExact(fname, knowledgePoint);
	
	// 第二步：加载筛选后的题目到链表
	QuestionNode* selectList = load("select.txt");
	if (selectList == NULL) {
		printf("没有符合条件的题目！\n");
		return;
	}
	
	// 统计题目总数
	int total = 0;
	QuestionNode* p = selectList;
	while (p != NULL) {
		total++;
		p = p->next;
	}
	
	if (total == 0) {
		printf("筛选结果为空！\n");
		freeList(selectList); // 确保释放内存
		return;
	}
	
	if (numQuestions > total) {
		printf("题库库存不足 %d，仅可生成 %d 道题目。\n", numQuestions, total);
		numQuestions = total;
	}
	
	// 动态分配指针数组（解决栈溢出问题）
	QuestionNode** questionArray = (QuestionNode**)malloc(total * sizeof(QuestionNode*));
	if (!questionArray) {
		printf("内存分配失败！\n");
		freeList(selectList); // 确保释放内存
		return;
	}
	
	// 将链表数据复制到数组中
	p = selectList;
	for (int i = 0; i < total; i++) {
		questionArray[i] = p;
		p = p->next;
	}
	
	// 随机抽题
	srand((unsigned int)time(NULL));
	int* used = (int*)calloc(total, sizeof(int)); // 动态分配 used 数组
	if (!used) {
		printf("内存分配失败！\n");
		free(questionArray);
		freeList(selectList); // 确保释放内存
		return;
	}
	
	QuestionNode* head = NULL, *tail = NULL;
	
	for (int i = 0; i < numQuestions; i++) {
		int index;
		do {
			index = rand() % total;
		} while (used[index]); // 确保没有重复选题
		used[index] = 1;
		
		QuestionNode* copy = (QuestionNode*)malloc(sizeof(QuestionNode));
		if (!copy) {
			printf("内存分配失败！\n");
			free(used);
			free(questionArray);
			freeList(selectList); // 确保释放内存
			return;
		}
		copy->data = questionArray[index]->data;
		copy->next = NULL;
		
		if (!head) {
			head = tail = copy;
		} else {
			tail->next = copy;
			tail = copy;
		}
	}
	
	// 重置 ID
	int cnt = 1;
	QuestionNode* t = head;
	while (t) {
		t->data.id = cnt++;
		t = t->next;
	}
	
	// 写入文件
	FILE* fout = fopen("select.txt", "w");
	if (!fout) {
		printf("无法写入文件！\n");
		free(used);
		free(questionArray);
		freeList(head); // 确保释放内存
		freeList(selectList); // 确保释放内存
		return;
	}
	
	t = head;
	while (t) {
		fprintf(fout, "%d %s %s %s %s %s %c %d %s\n", 
			t->data.id, t->data.questionText,
			t->data.options[0], t->data.options[1],
			t->data.options[2], t->data.options[3],
			t->data.correctOption, t->data.difficulty,
			t->data.knowledgePoint);
		t = t->next;
	}
	
	fclose(fout);
	printf("组卷完成，请开始答题！\n");
	
	// 释放资源
	free(used);
	free(questionArray);
	freeList(head); // 确保释放内存
	freeList(selectList); // 确保释放内存
	
	// 按难度排序（修改后的order函数需支持动态数组）
	order(numQuestions);
}


//显示试卷并统计分数
void showPaper()
{
	FILE *fp;
	fp = fopen("select.txt","r");
	if(fp == NULL)
	{
		printf("读取文件：select.txt 失败");
		exit(0);
	}
	
	int grade = 0,n = 0;
	int wrongQ[256] = {0};
	Question tmp;
	while(fscanf(fp,"%d %s %s %s %s %s %c %d %s\n",&tmp.id,tmp.questionText,tmp.options[0],tmp.options[1],tmp.options[2],tmp.options[3],&tmp.correctOption,&tmp.difficulty,tmp.knowledgePoint)!=EOF)
	{
		printf("%d.%s(5分）\n",tmp.id,tmp.questionText);
		printf("A.%s\n",tmp.options[0]);
		printf("B.%s\n",tmp.options[1]);
		printf("C.%s\n",tmp.options[2]);
		printf("D.%s\n",tmp.options[3]);
		printf("请输入你的答案：");
		getchar();
		char answer;
		scanf("%c",&answer);
		if(answer == tmp.correctOption)
			grade += 5;
		else
		{
			wrongQ[n++] = tmp.id;
		}
		printf("\n");
	}
	
	printf("错题编号为：");
	for(int j=0; j<n; j++)
		printf("%d",wrongQ[j]);
	printf("\n你的总分为：%d",grade);
	
	fclose(fp);
	
}

#include <stdio.h>
#include <stdlib.h>



int main() {
	int choice;
	
	while (1) {
		// 清屏前等待用户按回车
		clearScreen();
		
		// 显示菜单
		printf("\n ====== 考试系统菜单 ======\n");
		printf("1. 录入题库\n");
		printf("2. 查看题库\n");
		printf("3. 添加题目\n");
		printf("4. 删除题目\n");
		printf("5. 修改题目\n");
		printf("6. 组卷（知识点 + 数量）\n");
		printf("7. 开始考试\n");
		printf("0. 退出系统\n");
		printf("请选择操作编号：");
		scanf("%d", &choice);
		
		switch (choice) {
		case 1:
			input("questions.txt");
			break;
		case 2:
			lookThrough("questions.txt");
			break;
		case 3:
			printf("请选择操作：1. 插入新题目  2. 追加新题目\n");
			int addChoice;
			scanf("%d", &addChoice);
			if (addChoice == 1) {
				Insert("questions.txt");
			} else if (addChoice == 2) {
				appendQuestion("questions.txt");
			} else {
				printf("无效选择\n");
			}
			break;
		case 4:
			printf("请输入要删除的题目编号：");
			int deleteId;
			scanf("%d", &deleteId);
			delete(deleteId, "questions.txt");
			break;
		case 5:
			printf("请输入要修改的题目编号：");
			int modifyId;
			scanf("%d", &modifyId);
			modify("questions.txt", modifyId);
			break;
		case 6:
			printf("请输入组卷的知识点和题目数量：\n");
			char genKnowledge[MAX_KNOWLEDGE_LEN];
			int numQuestions;
			printf("知识点：");
			scanf("%s", genKnowledge);
			printf("题目数量：");
			scanf("%d", &numQuestions);
			generate("questions.txt", genKnowledge, numQuestions);
			break;
		case 7:
			showPaper();
			break;
		case 0:
			printf("退出系统\n");
			return 0;
		default:
			printf("无效操作，请重新选择\n");
		}
		
		// 执行完操作后，等待用户按回车再清屏
		printf("\n按回车键继续...");
		getchar();  // 读取之前遗留的换行符
		getchar();  // 等待用户按回车
	}
	
	return 0;
}


