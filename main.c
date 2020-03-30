#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 */
typedef struct
{
	char *name;
	int starting_time;
	int remaining_time;
} Process;
/*
 *
 */
typedef struct Node
{
	Process data;
	struct Node* next;
} Node;
/*
 *
 */
typedef struct
{
	Node *front;
	Node *rear;
} Queue;
/*
 *
 */
Queue* init()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}
/*
 *
 */
int isEmpty(Queue *queue)
{
	if(queue->front == NULL){
	return 1;
	}

	return 0;
}
/*
 *
 */
Process dequeue(Queue *queue)
{

	Node* temp = queue->front;
	Process process = queue->front->data;
	queue->front = queue->front->next;
	free(temp);
	process.remaining_time--;
	return process;
}
/*
 *
 */
void enqueue(Queue *queue, Process val)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = val;
	if(isEmpty(queue)){
		queue->front = newNode;
		queue->rear = newNode;
	}
	queue->rear->next = newNode;
	queue->rear = newNode;
}
/*
 *
 */
void destroy(Queue *queue)
{
	free(queue);
}
/*
*
*/
long int fileSize(char* filename)
{
	FILE* fp = fopen(filename, "r");
	fseek(fp, 0L, SEEK_END);
	long int res = ftell(fp);
	fclose(fp);
	return res;
}
/*
*
*/
int numOfLines(char* filename)
{
    FILE *fileptr;
    int count_lines = 0;
    char chr;

    fileptr = fopen(filename, "r");
    chr = getc(fileptr);
    while (chr != EOF){
        if (chr == '\n'){
            count_lines = count_lines + 1;
        }
        chr = getc(fileptr);
    }
    fclose(fileptr);
    return count_lines;
}
/*
*
*/
Process* fileToProcesses(char* filename)
{
        char *content = (char *) malloc(sizeof(char) * fileSize(filename));
        FILE *fptr = fopen(filename, "r");;
        char ch;
        int n = 0;
        ch = fgetc(fptr);

        while (ch != EOF){
                content[n] = ch;
                n++;
                ch = fgetc(fptr);
        }

        fclose(fptr);
        Process* processList = (Process*)malloc(sizeof(Process) * (numOfLines(filename)-1));
        int i=0;
        char* line;
        int j;
        while ((line = strtok_r(content, "\n", &content))){
                if(i>0){
                        j=0;
                        for(j;j<3;j++){
                                if(j==0){processList[i-1].name = strtok_r(line," ",&line);}
                                if(j==1){processList[i-1].starting_time = atoi(strtok_r(line," ",&line));}
                                if(j==2){processList[i-1].remaining_time = atoi(strtok_r(line," ",&line));}
                        }
                }
                i++;
        }
        return processList;

}
/*
*
*/
int getWatchingTime(char* filename)
{
        char *content = (char *) malloc(sizeof(char) * fileSize(filename));
        FILE *fptr = fopen(filename, "r");;
        char ch;
        int n = 0;
        ch = fgetc(fptr);

        while (ch != EOF){
                content[n] = ch;
                n++;
                ch = fgetc(fptr);
        }

        fclose(fptr);
	char* line = strtok_r(content, "\n", &content);
	strtok_r(line, "=", &line);
	return atoi(line);
}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{
	Queue* queue = init();
	int wTime = getWatchingTime(filename);
	Process* processes = fileToProcesses(filename);
	int procNum = numOfLines(filename)-1;
	int time;
	int i;
	for(time=0;time<wTime;time++){
		for(i=0;i<procNum;i++){
			if(processes[i].starting_time <= time && processes[i].remaining_time > 0){
				enqueue(queue,processes[i]);
				processes[i].remaining_time--;
			}
		}
		if(!isEmpty(queue)){
			Process dequeued = dequeue(queue);
			printf("%s\t(%d-->%d) ",dequeued.name,time,time+1);
			if(dequeued.remaining_time < 1){printf("%s aborts",dequeued.name);}
			printf("\n");
		}
		else{
			printf("idle\t(%d-->%d)\n",time,time+1);
		}
	}
}
/*
 *
 */
int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}
