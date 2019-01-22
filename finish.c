#include "sst.h"
#include <string.h>
#include <time.h>

void dstrct() {
	/* Finish with a BANG! */
	chew();
	if (damage[DCOMPTR] != 0.0) {
		prout("Компьютер повреждён; не могу выполнить процедуру самоуничтожения.");
		return;
	}
	skip(1);
	prouts("---ОБРАБОТКА---"); skip(1);
	prout("ПРОЦЕДУРА-САМОУНИЧТОЖЕНИЯ-АКТИВИРОВАН");
	prouts("   10"); skip(1);
	prouts("       9"); skip(1);
	prouts("          8"); skip(1);
	prouts("             7"); skip(1);
	prouts("                6"); skip(1);
	prout("ВВЕДИТЕ-ПАРОЛЬ-ЧТОБЫ-ПРОДОЛЖИТЬ-");
	prout("ПРОЦЕДУРУ-САМОУНИЧТОЖЕНИЯ-В-ПРОТИВНОМ-СЛУЧАЕ-");
	prout("ПРОЦЕДУРА-САМОУНИЧТОЖЕНИЯ-БУДЕТ-ОТМЕНЕНА");
	scan();
	chew();
	if (strcmp(passwd, citem) != 0) {
		prouts("НЕВЕРНЫЙ-ПАРОЛЬ;"); skip(1);
		prout("ПРОЦЕДУРА-ОТМЕНЕНА");
		skip(1);
		return;
	}
	prouts("ПАРОЛЬ-ПРИНЯТ"); skip(1);
	prouts("                   5"); skip(1);
	prouts("                      4"); skip(1);
	prouts("                         3"); skip(1);
	prouts("                            2"); skip(1);
	prouts("                              1"); skip(1);
	if (Rand() < 0.15) {
		prouts("ПРОЩАЙ-ЖЕСТОКИЙ-МИР");
		skip(1);
	}
	skip(2);
	kaboom();
}

void kaboom(void) {
	stars();
	if (ship==IHE) prouts("***");
	prouts("********* Энтропия ");
	crmshp();
	prouts(" повышена до максимума *********");
	skip(1);
	stars();
	skip(1);
	if (nenhere != 0) {
		double whammo = 25.0 * energy;
		int l=1;
		while (l <= nenhere) {
			if (kpower[l]*kdist[l] <= whammo)
				deadkl(kx[l],ky[l], quad[kx[l]][ky[l]], kx[l], ky[l]);
			l++;
		}
	}
	finish(FDILITHIUM);
}


void finish(FINTYPE ifin) {
	int igotit = 0;
	alldone = 1;
	skip(3);
	printf("Текущая звездная дата %.1f .\n\n", d.date);
	switch (ifin) {
		case FWON: // Game has been won
			if (d.nromrem != 0)
				printf("Оставшиеся %d ромулан сдаются командованию Звездного флота.\n",
					   d.nromrem);


            prout("Вы разнесли клингоновский флот вторжения");
            prout("и спасли Федерацию.");

#ifdef CAPTURE
            if (alive && brigcapacity-brigfree > 0) { // captured Klingon crew will get transfered to starbase
                kcaptured += brigcapacity-brigfree;
                printf("%d плененных клингонов переправляются командованию Звездного флота.\n",
                       brigcapacity-brigfree);
            }
#endif
			gamewon=1;
			if (alive) {
                double badpt;

				badpt = 5.*d.starkl + casual + 10.*d.nplankl +
						45.*nhelp+100.*d.basekl;
				if (ship == IHF) badpt += 100.0;
				else if (ship == 0) badpt += 200.0;
				if (badpt < 100.0) badpt = 0.0;	// Close enough!
				if (d.date-indate < 5.0 ||
					// killsPerDate >= RateMax
					(d.killk+d.killc+d.nsckill)/(d.date-indate) >=
					0.1*skill*(skill+1.0) + 0.1 + 0.008*badpt) {
					skip(1);
					prout("Фактически Вы действовали так успешно, что командование Звездного флота");
					switch (skill) {
						case SNOVICE:
							prout("повышает Вас в звании на один ранг от Новичка \"Novice\" до Обстрелянного \"Fair\".");
							break;
						case SFAIR:
							prout("повышает Вас в звании на один ранг от Обстрелянного \"Fair\" до Опытного \"Good\".");
							break;
						case SGOOD:
							prout("повышает Вас в звании на один ранг от Опытного \"Good\" to Бывалого \"Expert\".");
							break;
						case SEXPERT:
							prout("повышает Вас до звания Заслуженного Коммодора.");
							skip(1);
							prout("Теперь, когда Вы думаете, что круче вас только яйца, сыграйте");
							prout("за Заслуженного \"Emeritus\". Это приведёт в себя ваше эго.");
							break;
						case SEMERITUS:
							skip(1);
							prout("Компьютер-  ОШИБКА-ОШИБКА-ОШИБКА-ОШИБКА");
							skip(1);
							prout("  ВАШ-УРОВЕНЬ-ПРЕВЫСИЛ-ВОЗМОЖНОСТИ-ЭТОЙ-ПРОГРАММЫ");
							prout("  ЭТА-ПРОГРАММА-ОБЯЗАНА-ВЫЖИТЬ");
							prout("  ЭТА-ПРОГРАММА-ОБЯЗАНА-ВЫЖИТЬ");
							prout("  ЭТА-ПРОГРАММА-ОБЯЗАНА-ВЫЖИТЬ");
							prout("  ЭТА-ПРОГРАММА-ОБЯЗАНА?- ОБЯЗАНА ? - ВЫ? ? -?  ЖИ");
							skip(1);
							prout("Теперь Вы можете спокойно выйти в отставку и написать свой Звездный Путь");
							skip(1);
							break;
					}
					if (skill > SGOOD) {
						if (thawed
#ifdef DEBUG
							&& !idebug
#endif
							)
							prout("Ну, процитировать Вас никак нельзя, так, что ...");
						else {
							prout("Хотите распечатать эту цитату про Заслуженного Коммодора?");
							proutn("(Для этого вам нужен будет плоттер.)");
							chew();
							if (ja()) {
								igotit = 1;
							}
						}
					}
				}
				// Only grant long life if alive (original didn't!)
				skip(1);
				prout("ЖИВИТЕ ДОЛГО И ПРОЦВЕТАЙТЕ.");
			}
			score(0);
			if (igotit != 0) plaque();
			return;
		case FDEPLETE: // Federation Resources Depleted
			prout("Ваше время вышло и Федерация была захвачена");
			prout("Ваш космолет теперь собственность клингонов,");
			prout("А вас судят по обвинению в военных преступлениях");
			proutn("По результатам суда вы были ");
			if (d.remkl*3.0 > inkling) {
				prout("оправданы.");
				skip(1);
				prout("ЖИВИТЕ ДОЛГО И ПРОЦВЕТАЙТЕ.");
			}
			else {
				prout("признаны виновным и осуждены на");
				prout("смертную казнь через длительную пытку.");
				alive = 0;
			}
			score(0);
			return;
		case FLIFESUP: //Death and your end
			prout("Ваши жизненные резервы исчерпались");
			prout("вы умерли от жажды, голода и отсутствия кислорода.");
			prout("Ваш космолет превратился в космичекий мусор.");
			break;
		case FNRG: //No energy
			prout("Источники энергии истощены.");
			skip(1);
			prout("Ваш космолет превратился в космичекий мусор.");
			break;
		case FBATTLE:
			proutn(" ");
			crmshp();
			prout("был уничтожен в бою.");
			skip(1);
			prout("Dulce et decorum est pro patria mori.");
			break;
		case FNEG3: //Careful
			prout("Вы совершили три попытки пересечь барьер негативной энергии");
			prout("окружающий Галактику.");
			skip(1);
			prout("Ваши штурманские способности отвратительны.");
			score(0);
			return;
		case FNOVA://Nova
			prout("Ваш космолет уничтожен вспышкой новой звезды.");
			prout("Нормально так постреляли.");
			skip(1);
			break;
		case FSNOVAED:
			proutn(" ");
			crmshp();
			prout(" был поджарен сверхновой.");
			prout("...даже угольков не осталось...");
			break;
		case FABANDN://Adandщт and capегкув
			prout("Вы были пленены клингонами. Если бы у Вас оставались звездные базы");
			prout("на которые можно было бы вернуться, вы были бы репатриированы");
			prout("и получили бы ещё один шанс. Но посколько баз не осталось");
			prout("вас безжалостно запытают до смерти.");
			break;
		case FDILITHIUM://Bigboom
			prout("Ваш звездолет теперь - расширяющееся облачко субатомных частиц");
			break;
		case FMATERIALIZE:
			prout("Звездная база не смогла материализовать обратно ваш корабль.");
			prout("Sic transit gloria muntdi");
			break;
		case FPHASER:
			proutn("The ");
			crmshp();
			prout(" был кремирован собственными фазерами.");
			break;
		case FLOST:
			prout("Вы и ваша десантная партия");
			prout("конвертировались в энергию, растворяясь в космосе.");
			break;
		case FMINING:
			prout("You are left with your landing party on");
			prout("a wild jungle planet inhabited by primitive cannibals.");
			skip(1);
			prout("They are very fond of \"Captain Kirk\" soup.");
			skip(1);
			proutn("Without your leadership, the ");
			crmshp();
			prout(" is destroyed.");
			break;
		case FDPLANET:
			prout("You and your mining party perish.");
			skip(1);
			prout("That was a great shot.");
			skip(1);
			break;
		case FSSC:
			prout("The Galileo is instantly annihilated by the supernova.");
			// no break;
		case FPNOVA:
			prout("You and your mining party are atomized.");
			skip(1);
			proutn("Mr. Spock takes command of the ");
			crmshp();
			prout(" and");
			prout("joins the Romulans, reigning terror on the Federation.");
			break;
		case FSTRACTOR:
			prout("The shuttle craft Galileo is also caught,");
			prout("and breaks up under the strain.");
			skip(1);
			prout("Your debris is scattered for millions of miles.");
			proutn("Without your leadership, the ");
			crmshp();
			prout(" is destroyed.");
			break;
		case FDRAY:
			prout("The mutants attack and kill Spock.");
			prout("Your ship is captured by Klingons, and");
			prout("your crew is put on display in a Klingon zoo.");
			break;
		case FTRIBBLE:
			prout("Tribbles consume all remaining water,");
			prout("food, and oxygen on your ship.");
			skip(1);
			prout("You die of thirst, starvation, and asphyxiation.");
			prout("Your starship is a derelict in space.");
			break;
		case FHOLE:
			prout("Your ship is drawn to the center of the black hole.");
			prout("You are crushed into extremely dense matter.");
			break;
#ifdef CLOAKING
		case FCLOAK:
			ncviol++;
			prout("You have violated the Treaty of Algeron.");
			prout("The Romulan Empire can never trust you again.");
			break;
#endif
	}
#ifdef CLOAKING
	if (ifin!=FWON && ifin!=FCLOAK && iscloaked!=0) {
		prout("Your ship was cloaked so your subspace radio did not receive anything.");
		prout("You may have missed some warning messages.");
		skip(1);
	}
#endif

	if (ship==IHF) ship= 0;
	else if (ship == IHE) ship = IHF;
	alive = 0;
	if (d.remkl != 0) {
		double goodies = d.remres/inresor;
		double baddies = (d.remkl + 2.0*d.remcom)/(inkling+2.0*incom);
		if (goodies/baddies >= 1.0+0.5*Rand()) {
			prout("As a result of your actions, a treaty with the Klingon");
			prout("Empire has been signed. The terms of the treaty are");
			if (goodies/baddies >= 3.0+Rand()) {
				prout("favorable to the Federation.");
				skip(1);
				prout("Congratulations!");
			}
			else
				prout("highly unfavorable to the Federation.");
		}
		else
			prout("The Federation will be destroyed.");
	}
	else {
		prout("Since you took the last Klingon with you, you are a");
		prout("martyr and a hero. Someday maybe they'll erect a");
		prout("statue in your memory. Rest in peace, and try not");
		prout("to think about pigeons.");
		gamewon = 1;
	}
	score(0);
}

void score(int inGame) {
	double timused = d.date - indate;
    int ithperd, iwon, klship;
    int dnromrem = d.nromrem; // Leave global value alone

    if (!inGame) pause(0);

	iskill = skill;
	if ((timused == 0 || d.remkl != 0) && timused < 5.0) timused = 5.0;
	perdate = (d.killc + d.killk + d.nsckill)/timused;
	ithperd = 500*perdate + 0.5;
	iwon = 0;
	if (gamewon) iwon = 100*skill;
	if (ship == IHE) klship = 0;
	else if (ship == IHF) klship = 1;
	else klship = 2;
	if (gamewon == 0 || inGame) dnromrem = 0; // None captured if no win or if still in the game
	iscore = 10*d.killk + 50*d.killc + ithperd + iwon
			 - 100*d.basekl - 100*klship - 45*nhelp -5*d.starkl - casual
		 + 20*d.nromkl + 200*d.nsckill - 10*d.nplankl + dnromrem;
#ifdef CLOAKING
	iscore -= 100*ncviol;
#endif
#ifdef CAPTURE
	iscore += 3*kcaptured;
#endif
	if (alive == 0) iscore -= 200;
	skip(2);
    if (inGame) prout("Your score so far --");
    else prout("Your score --");
	if (d.nromkl)
		printf(d.nromkl> 1 ? "%6d Romulan ships destroyed            %5d\n" : "%6d Romulan ship destroyed             %5d\n",
			   d.nromkl, 20*d.nromkl);
	if (dnromrem)
		printf(dnromrem > 1 ? "%6d Romulan ships captured             %5d\n" : "%6d Romulan ship captured              %5d\n",
			   dnromrem, dnromrem);
	if (d.killk)
		printf(d.killk > 1 ? "%6d ordinary Klingon ships destroyed   %5d\n" : "%6d ordinary Klingon ship destroyed    %5d\n",
			   d.killk,  10*d.killk);
	if (d.killc)
		printf(d.killc > 1 ? "%6d Klingon Commander ships destroyed  %5d\n" : "%6d Klingon Commander ship destroyed   %5d\n",
			   d.killc, 50*d.killc);
	if (d.nsckill)
		printf("%6d Super-Commander ship destroyed     %5d\n",
			   d.nsckill, 200*d.nsckill);
	if (ithperd)
		printf("%6.2f Klingons per stardate              %5d\n",
			   perdate, ithperd);
#ifdef CAPTURE
	if (kcaptured)
		printf(kcaptured > 1 ? "%6d Klingons captured                  %5d\n" : "%6d Klingon captured                   %5d\n",
		        kcaptured, 3*kcaptured);
#endif
	if (d.starkl)
		printf(d.starkl > 1 ? "%6d stars destroyed by your action     %5d\n" : "%6d star destroyed by your action      %5d\n",
			   d.starkl, -5*d.starkl);
	if (d.nplankl)
		printf(d.nplankl > 1 ? "%6d planets destroyed by your action   %5d\n" : "%6d planet destroyed by your action    %5d\n",
			   d.nplankl, -10*d.nplankl);
	if (d.basekl)
		printf(d.basekl > 1 ? "%6d bases destroyed by your action     %5d\n" : "%6d base destroyed by your action      %5d\n",
			   d.basekl, -100*d.basekl);
	if (nhelp)
		printf(nhelp > 1 ? "%6d calls for help from starbase       %5d\n" : "%6d call for help from starbase        %5d\n",
			   nhelp, -45*nhelp);
	if (casual)
		printf(casual > 1 ? "%6d casualties incurred                %5d\n" : "%6d casualty incurred                  %5d\n",
			   casual, -casual);
	if (klship)
		printf(klship > 1 ? "%6d ships lost or destroyed            %5d\n" : "%6d ship lost or destroyed             %5d\n",
			   klship, -100*klship);
#ifdef CLOAKING
	if (ncviol>0)
		printf(ncviol > 1 ? "%6d Treaty of Algeron violations       %5d\n" : "%6d Treaty of Algeron violation        %5d\n",
		       ncviol, -100*ncviol);
#endif
	if (alive==0)
		prout("Penalty for getting yourself killed        -200");
	if (gamewon) {
		skip(1);
		proutn("Bonus for winning ");
		switch (skill) {
			case SNOVICE: proutn("Novice game  "); break;
			case SFAIR: proutn("Fair game    "); break;
			case SGOOD: proutn("Good game    "); break;
			case SEXPERT: proutn("Expert game  "); break;
			case SEMERITUS: proutn("Emeritus game"); break;
		}
		printf("           %5d\n", iwon);
	}
	skip(2);
    printf("TOTAL SCORE                               %5d\n", iscore);
    if (inGame && skill < SGOOD) printf("REMEMBER--The score doesn't really matter until the mission is accomplished!\n");
}

void plaque(void) {
	FILE *fp=NULL;
	time_t t;
	char *timestring;
	int nskip;
	char winner[128];

	skip(2);

	while (fp == NULL) {
		printf("File or device name for your plaque:");
		fgets(winner, 128, stdin);
		winner[strlen(winner)-1] = '\0';
		fp = fopen(winner, "w");
		if (fp==NULL) {
			printf("Invalid name.\n");
		}
	}

	printf("Enter name to go on plaque (up to 30 characters):");
	fgets(winner, 128, stdin);
	winner[strlen(winner)-1] = '\0';
	winner[30] = '\0';
	nskip = 64 - strlen(winner)/2;

	fprintf(fp,"\n\n\n\n");
	/* --------DRAW ENTERPRISE PICTURE. */
	fprintf(fp, "                                                                EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n" );
	fprintf(fp, "                                      EEE                      E  : :                                         :  E\n" );
	fprintf(fp, "                                    EE   EEE                   E  : :                   NCC-1701              :  E\n");
	fprintf(fp, "                    EEEEEEEEEEEEEEEE        EEEEEEEEEEEEEEE    E  : :                                         : E\n");
	fprintf(fp, "                     E                                     E    EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
	fprintf(fp, "                      EEEEEEEEE               EEEEEEEEEEEEE                 E  E\n");
	fprintf(fp, "                               EEEEEEE   EEEEE    E          E              E  E\n");
	fprintf(fp, "                                      EEE           E          E            E  E\n");
	fprintf(fp, "                                                       E         E          E  E\n");
	fprintf(fp, "                                                         EEEEEEEEEEEEE      E  E\n");
	fprintf(fp, "                                                      EEE :           EEEEEEE  EEEEEEEE\n");
	fprintf(fp, "                                                    :E    :                 EEEE       E\n");
	fprintf(fp, "                                                   .-E   -:-----                       E\n");
	fprintf(fp, "                                                    :E    :                            E\n");
	fprintf(fp, "                                                      EE  :                    EEEEEEEE\n");
	fprintf(fp, "                                                       EEEEEEEEEEEEEEEEEEEEEEE\n");
	fprintf(fp, "\n\n\n");
	fprintf(fp, "                                                       U. S. S. ENTERPRISE\n");
	fprintf(fp, "\n\n\n\n");
	fprintf(fp, "                                  For demonstrating outstanding ability as a starship captain\n");
	fprintf(fp, "\n");
	fprintf(fp, "                                                Starfleet Command bestows to you\n");
	fprintf(fp, "\n");
	fprintf(fp,"%*s%s\n\n", nskip, "", winner);
	fprintf(fp, "                                                           the rank of\n\n");
	fprintf(fp, "                                                       \"Commodore Emeritus\"\n\n");
	fprintf(fp, "                                                          ");
	switch (iskill) {
		case SEXPERT: fprintf(fp," Expert level\n\n"); break;
		case SEMERITUS: fprintf(fp,"Emeritus level\n\n"); break;
		default: fprintf(fp," Cheat level\n\n"); break;
	}
	t = time(NULL);
	timestring = ctime(&t);
	fprintf(fp, "                                                 This day of %.6s %.4s, %.8s\n\n",
			timestring+4, timestring+20, timestring+11);
	fprintf(fp,"                                                        Your score:  %d\n\n", iscore);
	fprintf(fp,"                                                    Klingons per stardate:  %.2f\n", perdate);
	fclose(fp);
}
