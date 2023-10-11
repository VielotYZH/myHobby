#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#define NODE_NUMBER 7
#define INFINITY 1024
#define INFO_SIZE sizeof(RoutingInfo)

typedef struct
{
	int destination;
	int cost;
	int nextHop;
} RoutingInfo;

RoutingInfo routingTable[NODE_NUMBER][NODE_NUMBER];
int ifChanged = 1;

void initRoutingTable(FILE *fp)
{
	int token, sn;
	sn = fscanf(fp, "%d", &token);
	for (int i = 0; i < NODE_NUMBER; i++)
	{
		for (int j = 0; j < NODE_NUMBER; j++)
		{
			if (sn != 0)
			{
				routingTable[i][j].cost = token;
			}
			else
			{
				char buffer;
				fscanf(fp, "%c", &buffer);
				if (buffer == 'i')
				{
					routingTable[i][j].cost = INFINITY;
				}
			}
			routingTable[i][j].destination = j;
			routingTable[i][j].nextHop = routingTable[i][j].cost == INFINITY || routingTable[i][j].cost == 0 ? NODE_NUMBER : j;
			sn = fscanf(fp, "%d", &token);
		}
	}
}

void receiveRoutingTable(int senderNum, int receiverNum, int senderInfo[])
{
	for (int i = 0; i < NODE_NUMBER; i++)
	{
		if (routingTable[receiverNum][i].cost > routingTable[receiverNum][senderNum].cost + senderInfo[i])
		{
			routingTable[receiverNum][i].cost = routingTable[receiverNum][senderNum].cost + senderInfo[i];
			routingTable[receiverNum][i].nextHop = senderNum;
			ifChanged = 1;
		}
	}
}

int isAdjacent(int nodeNum1, int nodeNum2)
{
	for (int i = 0; i < NODE_NUMBER; i++)
	{
		if (routingTable[nodeNum1][i].nextHop == nodeNum2)
		{
			return 1;
		}
	}
	return 0;
}

void sendRoutingTable()
{
	int senderInfos[NODE_NUMBER][NODE_NUMBER];
	for (int i = 0; i < NODE_NUMBER; i++)
	{
		for (int j = 0; j < NODE_NUMBER; j++)
		{
			senderInfos[i][j] = routingTable[i][j].cost;
		}
	}
	for (int i = 0; i < NODE_NUMBER; i++)
	{
		for (int j = 0; j < NODE_NUMBER; j++)
		{
			if (isAdjacent(i, j))
			{
				receiveRoutingTable(i, j, senderInfos[i]);
			}
		}
	}
}

void main()
{
	FILE *fp = fopen("test.txt", "r");
	initRoutingTable(fp);
	fclose(fp);
	while (ifChanged)
	{
		ifChanged = 0;
		sendRoutingTable();
		for (int i = 0; i < NODE_NUMBER; i++)
		{
			for (int j = 0; j < NODE_NUMBER; j++)
			{
				if (routingTable[i][j].cost == INFINITY)
				{
					printf("%c ", '*');
				}
				else
				{
					printf("%d ", routingTable[i][j].cost);
				}
			}
			printf("\n");
		}
		printf("--------------------------------------\n");
	}
}