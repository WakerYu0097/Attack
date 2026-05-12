#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef enum
{
    Ch_Thuder,
    Ch_Fly
} CharacterType;

typedef enum
{
    Thunder,
    Fly,
    Super,
    Normal
} AttackType;

typedef enum
{
    Attack,//对对方造成伤害
    Accelerate,//提升己方速度
    Opp_Atk_down,//降低对方攻击力
    Opp_Accuracy_down//降低对方命中率
} AttackEffect;

typedef struct
{
    int Atk_Down_Turns;//攻击下降叠加层数
    float Atk_Down;//攻击下降数值
    int Speed_Up_Turns;//速度提升叠加层数
    float Speed_Up;//速度提升数值
    int Accuracy_Down_Turns;//命中率下降叠加层数
    float Accuracy_Down;//命中率下降数值
} CharacterStatus;

typedef struct
{
    char name[20];
    AttackType type;
    AttackEffect effect;
    float power;
    float accuracy;
    int count;
} Skill;

typedef struct
{
    int BallCount;
    int PotionCount;
} Items;

typedef struct
{
    char name[20];
    CharacterType type;
    int level;
    float attackPower;
    float defensePower;
    float maxHealth;
    float health;
    float speed;
    Skill skills[4];
    CharacterStatus status;
} Character;

bool check_hit(float accuracy);
void UseItems(Items* items,Character* self, Character* target);
void UseSkills(Character* self, Character* target);
void MyTurn(Character* self, Character* target,Items* items);
void BoboTurn(Character* self, Character* target);
void Tick(Character* main,Character* opp,Items* items);

//成功捕捉标志
bool Complete=false;

int main()
{
    srand(time(NULL));
    printf("  _ _ _ _ _ _ _ _ _ _ _ _ \n");
	printf(" |                       |\n");
	printf(" |    欢迎进入游戏       |\n");
	printf(" |      宝可梦 黄        |\n");
	printf(" |  请按任意键开始游戏   |\n");
	printf(" |                       |\n");
	printf("  - - - - - - - - - - - --\n");
    getchar();
    printf("大木博士：欢迎来到宝梦可的世界，我是大木博士。\n");
	printf("大木博士：在这个世界中有一种神奇的生物，叫宝梦可，可以帮助我们进行各种各样的工作\n");
	char name[11];
	printf("大木博士：你的英文名字是（十个字母以内）：\n");
    scanf("%s",name);
    printf("原来你是%s啊，那事不宜迟，我们来学习一下如何捕捉宝可梦吧!\n", name);
    char ch;
    printf("是否进入程序（Y/N）\n");
    getchar();
    while (1) 
    {
        scanf("%c", &ch);
        if (ch == 'Y')
        {
            printf("那就进入教学吧!\n");
            break;
        }
        else
        {
            if (ch == 'N')
            {
                printf("大木博士：不想学习吗，那就再见吧!\n");
                return 0;
            }
            else
            {
                printf("大木博士：别拿我开玩笑！\n");
                printf("是否进入程序（Y/N）\n");
            }
        }
    }

    Character pikachu={
        .name="皮卡丘",
        .level=50,
        .type=Ch_Thuder,
        .maxHealth=100.0,
        .health=100.0,
        .attackPower=55.0,
        .defensePower=40.0,
        .speed=90.0,
        .status={
            .Atk_Down_Turns=0,
            .Atk_Down=0.0
        },
        .skills={
            {
                .name="电击",
                .type=Thunder,
                .effect=Attack,
                .power=40.0,
                .accuracy=1.0,
                .count=30
            },
            {
                .name="摇尾巴",
                .type=Normal,
                .effect=Opp_Atk_down,
                .power=0.0,
                .accuracy=1.0,
                .count=30
            },
            {
                .name="电光一闪",
                .type=Normal,
                .effect=Attack,
                .power=40.0,
                .accuracy=1.0,
                .count=30
            },
            {
                .name="十万伏特",
                .type=Thunder,
                .effect=Attack,
                .power=90.0,
                .accuracy=1.0,
                .count=15
            }
        }
    };
    Character bobo={
        .name="波波",
        .level=50,
        .type=Ch_Fly,
        .maxHealth=1100.0,
        .health=1100.0,
        .attackPower=45.0,
        .defensePower=40.0,
        .speed=56.0,
        .status={
            .Atk_Down_Turns=0,
            .Atk_Down=0.0
        },
        .skills={
            {
                .name="撞击",
                .type=Normal,
                .effect=Attack,
                .power=40.0,
                .accuracy=1.0,
                .count=35
            },
            {
                .name="泼沙",
                .type=Normal,
                .effect=Opp_Accuracy_down,
                .power=0.0,
                .accuracy=1.0,
                .count=30
            },
            {
                .name="高速移动",
                .type=Super,
                .effect=Accelerate,
                .power=0.0,
                .accuracy=1.0,
                .count=30
            },
            {
                .name="起风",
                .type=Fly,
                .effect=Attack,
                .power=40.0,
                .accuracy=1.0,
                .count=30
            }
        }
    };
    Items items={
        .BallCount=10,
        .PotionCount=5
    };

    printf("野生%s出来了！\n", bobo.name);
    printf("你派出了%s！\n",pikachu.name);

    while((pikachu.health>0 && bobo.health>0) && !Complete)
    {
        Tick(&pikachu, &bobo,&items);
    }
    printf("战斗结束！\n");
    return 0;
}

bool check_hit(float accuracy)
{
    float rand_value = (float)rand() / RAND_MAX;
    return rand_value <= accuracy;
}

bool UseBall(Items* items, Character* target)
{
    if (items->BallCount <= 0)
    {
        printf("没有精灵球了！\n");
        return false;
    }
    if(target->health <= 0)
    {
        printf("宝可梦已经昏迷，无法捕捉！\n");
        return false;
    }
    items->BallCount--;
    float accuracy;
    if(target->health/target->maxHealth > 0.5)
        accuracy = 0.2;
    else if(target->health/target->maxHealth > 0.2)
        accuracy = 0.5;
    else
        accuracy = 0.9;
    if (check_hit(accuracy))
    {
        printf("捕捉成功！\n");
        Complete=true;
        target->health=0;
        return true;
    }
    else
    {
        printf("捕捉失败！\n");
        return false;
    }
}
bool UsePotion(Items* items, Character* target)
{
    if (items->PotionCount <= 0)
    {
        printf("没有伤药了！\n");
        return false;
    }
    items->PotionCount--;
    float heal_amount;
    float temp_health=target->health + 100.0;
    if(temp_health>target->maxHealth)
    {
        heal_amount=target->maxHealth - target->health;
        target->health=target->maxHealth;
    }
    else
    {
        heal_amount=100.0;
        target->health+=100.0;
    }
    printf("你使用了伤药\n");
    printf("%s的血量恢复了%f点!\n", target->name, heal_amount);
    return true;
}

void UseItems(Items* items,Character* self, Character* target)
{
    printf("你想使用的道具是:\n");
    printf("1:精灵球 (剩余数量: %d)\n", items->BallCount);
    printf("2:伤药 (剩余数量: %d)\n", items->PotionCount);
    char item_choice;
    getchar(); // 清除缓冲区的换行符
    scanf("%c", &item_choice);
    switch (item_choice)
    {
        case '1':
            printf("你选择了使用精灵球！\n");
            UseBall(items, target);
            break;
        case '2':
            printf("你选择了使用伤药！\n");
            UsePotion(items, self);
            break;
        default:
            printf("无效的道具选择！\n");
            printf("你错失了一个回合！\n");
            break;
    }
}

void UseSkill(Character* self,Character* target, int skill_index)
{
    Skill* skill = &self->skills[skill_index];
    if (skill->count <= 0)
    {
        printf("技能%s的可用次数已用尽！\n", skill->name);
        return;
    }
    skill->count--;
    if (!check_hit(skill->accuracy))
    {
        printf("%s发动技能%s未命中！\n",self->name, skill->name);
        return;
    }

    //先处理一般攻击效果
    if (skill->effect == Attack)
    {
        float damage=((self->level *0.4+2)*skill->power*self->attackPower/target->defensePower/50+2);
        if(target->type==Ch_Fly && skill->type==Thunder)
            damage*=2.0;
        float original_health=target->health;
        damage=(damage-self->status.Atk_Down)<0?0:(damage -self->status.Atk_Down);
        if(check_hit(skill->accuracy - self->status.Accuracy_Down))
            target->health -= damage;
        if (target->health < 0)
            target->health = 0;
        printf("%s使用了技能%s，对%s造成了%.0f点伤害！\n", self->name, skill->name, target->name,original_health - target->health);
    }
    //处理降低攻击效果
    else if(skill->effect == Opp_Atk_down)
    {
        if(target->status.Atk_Down_Turns>=4)
        {
            printf("%s的攻击力不能再降低了！\n",target->name);
            return;
        }
        target->status.Atk_Down_Turns++;
        target->status.Atk_Down += 5.0;
    }
    //处理降低命中效果
    else if(skill->effect == Opp_Accuracy_down)
    {
        if(target->status.Accuracy_Down_Turns>=4)
        {
            printf("%s的命中率不能再降低了！\n",target->name);
            return;
        }
        target->status.Accuracy_Down_Turns++;
        target->status.Accuracy_Down += 0.1;
    }
    //处理提升速度效果
    else if(skill->effect == Accelerate)
    {
        if(self->status.Speed_Up_Turns>=4)
        {
            printf("%s的速度不能再提升了！\n",self->name);
            return;
        }
        self->status.Speed_Up_Turns++;
        self->status.Speed_Up += 10.0;
    }
    else
    {
        printf("未知的技能效果！\n");
    }
}

void UseSkills(Character* self,Character* target)
{
    printf("你想选择的技能是:\n");
    for(int i=0;i<4;i++)
    {
        char type_str[10];
        if(self->skills[i].type==Thunder)
            strcpy(type_str, "电");
        else if(self->skills[i].type==Fly)
            strcpy(type_str, "飞行");
        else if(self->skills[i].type==Super)
            strcpy(type_str, "超能力");
        else
            strcpy(type_str, "一般");

        printf("%d:%-10s属性:%-6s威力:%-6.0f命中:%-6.0f可用次数:%-2d\n",i+1,self->skills[i].name,type_str,self->skills[i].power,self->skills[i].accuracy*100,self->skills[i].count);
    }
    char skill_choice;
    getchar(); // 清除缓冲区的换行符
    scanf("%c", &skill_choice);
    switch (skill_choice)
    {
        case '1':
            printf("你选择了使用技能%s！\n",self->skills[0].name);
            UseSkill(self, target, 0);
            break;
        case '2':
            printf("你选择了使用技能%s！\n",self->skills[1].name);
            UseSkill(self, target, 1);
            break;
        case '3':
            printf("你选择了使用技能%s！\n",self->skills[2].name);
            UseSkill(self, target, 2);
            break;
        case '4':
            printf("你选择了使用技能%s！\n",self->skills[3].name);
            UseSkill(self, target, 3);
            break;
        default:
            printf("无效的技能选择！\n");
            printf("你错失了一个回合！\n");
            break;
    }
}

void MyTurn(Character* self, Character* target,Items* items)
{
    printf("你的选择是？\n");
    printf("1:使用道具\n");
    printf("2:使用技能\n");
    char choice;
    getchar(); // 清除缓冲区的换行符
    scanf("%c", &choice);
    switch (choice)
    {
        case '1':
            printf("你选择了使用道具！\n");
            UseItems(items, self, target);
            break;
        case '2':
            printf("你选择了使用技能！\n");
            UseSkills(self, target);
            break;
        default:
            printf("无效的选择！\n");
            printf("你错失了一个回合！\n");
            break;
    }
}

void BoboTurn(Character* self, Character* target)
{
    printf("%s的回合！\n", self->name);
    int skill_index = rand() % 4;
    printf("%s选择了使用技能%s！\n", self->name, self->skills[skill_index].name);
    UseSkill(self, target, skill_index);
}

void Tick(Character* main,Character* opp,Items* items)
{
    //一次回合开始
    printf("%sHP：%.0f\n%sHP：%.0f\n",main->name,main->health,opp->name,opp->health);
    if(main->speed>=opp->speed)
    {
        MyTurn(main, opp,items);
        if(opp->health>0)
            BoboTurn(opp, main);
    }
    else
    {
        BoboTurn(opp,main);
        if(main->health>0)
            MyTurn(main, opp,items);
    }
}

