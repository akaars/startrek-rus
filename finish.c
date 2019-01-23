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
			prout("Вы и Ваша десантная партия - на дикой, покрытой джунглями планете");
			prout("населенной примитивными каннибалами.");
			skip(1);
			prout("Каннибалам очень нравится суп \"Капитан Кирк\".");
			skip(1);
			proutn("Оставшийся без Вашего руководства ");
			crmshp();
			prout(" уничтожен.");
			break;
		case FDPLANET:
			prout("Вы и Ваша шахтёрская партия погибли.");
			skip(1);
			prout("Нормально так стрельнули.");
			skip(1);
			break;
		case FSSC:
			prout("Галилео мгновенно аннигилировал во взрыве сверхновой.");
			// no break;
		case FPNOVA:
			prout("Вы и Ваша шахтёрская партия распылены на атомы.");
			skip(1);
			proutn("Спок принимает команду над ");
			crmshp();
			prout(" и");
			prout("присоединяется к ромуланам, сея ужас в Федерации.");
			break;
		case FSTRACTOR:
			prout("Посадочный челнок Галилей так же захвачен,");
			prout("и разрушается от нагрузок.");
			skip(1);
			prout("Ваш мусор рассеян на миллионы километров.");
			proutn("Оставшийся без Вашего руководства ");
			crmshp();
			prout(" уничтожен.");
			break;
		case FDRAY:
			prout("Мутанты атакуют и убивают Спока.");
			prout("Ваш корабль захвачен клингонами и");
			prout("его экипаж демонстрируется в клингоновском зоопарке.");
			break;
		case FTRIBBLE:
			prout("Триблы выпили всю оставшуюся воду,");
			prout("сожрали еду и выдышали весь оставшийся кислород на корабле.");
			skip(1);
			prout("Вы погибаете от жажды, голода и удушья.");
			prout("Ваш звездолет покинут в космосе.");
			break;
		case FHOLE:
			prout("Ваш корабль затянут в центр черной дыры.");
			prout("Вы разбиваетесь о сверхплотную материю.");
			break;
#ifdef CLOAKING
		case FCLOAK:
			ncviol++;
			prout("Вы нарушили Алджеронский договор.");
			prout("Ромуланская Империя никогда больше не сможет вам доверять.");
			break;
#endif
	}
#ifdef CLOAKING
	if (ifin!=FWON && ifin!=FCLOAK && iscloaked!=0) {
		prout("Ваш корабль был замаскирован, из-за чего подпространственное радио не работало.");
		prout("Возможно вы пропустили предупреждения.");
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
			prout("В результате ваших действий был подписан мирный договор");
			prout("с Империей клингонов. Условия договора");
			if (goodies/baddies >= 3.0+Rand()) {
				prout("выгодня для Федерации.");
				skip(1);
				prout("Поздравляем!");
			}
			else
				prout("очень невыгодны для Федерации.");
		}
		else
			prout("Федерация будет уничтожена.");
	}
	else {
		prout("Поскольку Вы забрали последнего клингона с собой, вы ");
		prout("и мученик, и герой. Возможно, когда-нибудь");
		prout("в Вашу честь воздвигнут статую. Покойтесь с миром и старайтесь");
		prout("не думать о голубях.");
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
    if (inGame) prout("Ваш текущий счет --");
    else prout("Ваш счет --");
	if (d.nromkl)
		printf(d.nromkl> 1 ? "%6d кораблей ромулан уничтожено            %5d\n" : "%6d корабль ромулан уничтожен             %5d\n",
			   d.nromkl, 20*d.nromkl);
	if (dnromrem)
		printf(dnromrem > 1 ? "%6d кораблей ромулан захвачено             %5d\n" : "%6d корабль ромулан захвачен              %5d\n",
			   dnromrem, dnromrem);
	if (d.killk)
		printf(d.killk > 1 ? "%6d кораблей клингонов уничтожено   %5d\n" : "%6d корабль клингонов уничтожен    %5d\n",
			   d.killk,  10*d.killk);
	if (d.killc)
		printf(d.killc > 1 ? "%6d Коммандеров клингонов уничтожено  %5d\n" : "%6d Коммандер клингонов уничтожен   %5d\n",
			   d.killc, 50*d.killc);
	if (d.nsckill)
		printf("%6d Супер-коммандер уничтожен     %5d\n",
			   d.nsckill, 200*d.nsckill);
	if (ithperd)
		printf("%6.2f Клингонов в сутки              %5d\n",
			   perdate, ithperd);
#ifdef CAPTURE
	if (kcaptured)
		printf(kcaptured > 1 ? "%6d клингонов пленено                  %5d\n" : "%6d клингоно пленен                   %5d\n",
		        kcaptured, 3*kcaptured);
#endif
	if (d.starkl)
		printf(d.starkl > 1 ? "%6d звезд уничтожено вашими действиями     %5d\n" : "%6d звезда уничтожена вашими действиями      %5d\n",
			   d.starkl, -5*d.starkl);
	if (d.nplankl)
		printf(d.nplankl > 1 ? "%6d планет уничтожено вашими действиями   %5d\n" : "%6d планета уничтожена вашими действиями    %5d\n",
			   d.nplankl, -10*d.nplankl);
	if (d.basekl)
		printf(d.basekl > 1 ? "%6d баз уничтожено вашими действиями     %5d\n" : "%6d база уничтожена вашими действиями      %5d\n",
			   d.basekl, -100*d.basekl);
	if (nhelp)
		printf(nhelp > 1 ? "%6d запросов помощи у звездной базы       %5d\n" : "%6d запрос помощи у звездной базы        %5d\n",
			   nhelp, -45*nhelp);
	if (casual)
		printf(casual > 1 ? "%6d погибших                 %5d\n" : "%6d погибший                  %5d\n",
			   casual, -casual);
	if (klship)
		printf(klship > 1 ? "%6d кораблей потеряно, или уничтожено            %5d\n" : "%6d  корабль потерян, или уничтожен             %5d\n",
			   klship, -100*klship);
#ifdef CLOAKING
	if (ncviol>0)
		printf(ncviol > 1 ? "%6d нарушений Алджеронского договора       %5d\n" : "%6d нарушение Алджеронского договора        %5d\n",
		       ncviol, -100*ncviol);
#endif
	if (alive==0)
		prout("Штрафные очки за то, что позволили себя убить        -200");
	if (gamewon) {
		skip(1);
		proutn("Бонус за победу ");
		switch (skill) {
			case SNOVICE: proutn("Игра за новичка  "); break;
			case SFAIR: proutn("Игра за обстрелянного    "); break;
			case SGOOD: proutn("Игра за опытного    "); break;
			case SEXPERT: proutn("Игра за бывалого  "); break;
			case SEMERITUS: proutn("Игра за Заслуженного"); break;
		}
		printf("           %5d\n", iwon);
	}
	skip(2);
    printf("ОБЩИЙ СЧЕТ                               %5d\n", iscore);
    if (inGame && skill < SGOOD) printf("ПОМНИТЕ--счет не имеет смысла, пока миссия не завершена!\n");
}

void plaque(void) {
	FILE *fp=NULL;
	time_t t;
	char *timestring;
	int nskip;
	char winner[128];

	skip(2);

	while (fp == NULL) {
		printf("Имя файла или устройства для сохранения награды:");
		fgets(winner, 128, stdin);
		winner[strlen(winner)-1] = '\0';
		fp = fopen(winner, "w");
		if (fp==NULL) {
			printf("Неверно указанное имя.\n");
		}
	}

	printf("Введите имя для указания на награде (до 30 знаков):");
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
