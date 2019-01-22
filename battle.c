	#include "sst.h"

#ifdef CLOAKING
void cloak(void) {
	int key;
	enum {NONE, CLON, CLOFF} action = NONE;

	if (ship == IHF) {
		prout("У Королевы Фей нет маскировочного модуля.");
		return;
	}

	key = scan();

	if (key == IHREAL) return;

	if (key == IHALPHA) {
		if (isit("on")) {
			if (iscloaked) {
				prout("Маскировочный модуль включен.");
				return;
			}
			action = CLON;
		}
		else if (isit("off")) {
			if (!iscloaked) {
				prout("Маскировочный модуль выключен.");
				return;
			}
			action = CLOFF;
		}
		else {
			huh();
			return;
		}
	} else {
		if (!iscloaked) {
			proutn("Включить маскировочный модуль?");
			if (ja()==0) return;
			action = CLON;
		}
		if (iscloaked) {
			proutn("Выключить маскировочный модуль?");
			if (ja()==0) return;
			action = CLOFF;
		}
		if (action == NONE) return;
	}

    if (action==CLOFF) {
        if (irhere && d.date >= ALGERON && !isviolreported) {
            prout("Спок- \"Капитан, Алджеронский договор в силе.\n   Вы уверены, что это мудрый поступок?\"");
            if (ja() == 0) return;
        }
		prout("Инженер Скотт- \"Так точно, сэр.\"");
        iscloaked = FALSE;
        if (irhere && d.date >= ALGERON && !isviolreported) {
            prout("Ромуланский корабль обнаружил нарушение Алджеронского договора!");
            ncviol++;
            isviolreported = TRUE;
        }

//        if (neutz && d.date >= ALGERON) finish(FCLOAK);
		return;
	}

	if (damage[DCLOAK]!=0) {
		prout("Инженер Скотт- \"Маскирующий модуль повреждён, Сэр.\"");
		return;
	}

	if (condit==IHDOCKED) {
		prout("Вы не можете маскироваться будучи пристыкованным.");
		return;
	}

	if (d.date >= ALGERON && !isviolreported)
	{
		prout("Спок- \"Капитан, использование маскирующего модуля есть нарушение");
		prout("  Алджеронского договора. Рассматриваю альтернативы,");
		proutn("  вы уверены, что это мудрый поступок?");
		if (ja()==0) return;
	}

	prout("Инжерен Скотт- \"Маскирующий модуль активирован, сэр.\"");
	iscloaking = TRUE;
    if (irhere && d.date >= ALGERON && !isviolreported) {
        prout("Ромуланский корабль обнаружил, что вы нарушаете Алджеронский договор!!");
        ncviol++;
        isviolreported = TRUE;
    }
}
#endif

void sheild(int i) {
	int key;
	enum {NONE, SHUP, SHDN, NRG} action = NONE;

	ididit = 0;

	if (i == 2) action = SHUP;
	else {
		key = scan();
		if (key == IHALPHA) {
			if (isit("transfer"))
				action = NRG;
			else {
				chew();
				if (damage[DSHIELD]) {
					prout("Щиты повреждены и отключены.");
					return;
				}
				if (isit("up"))
					action = SHUP;
				else if (isit("down"))
					action = SHDN;
			}
		}
		if (action==NONE) {
			proutn("Хотите изменить энергию выделяемую на щиты? ");
			if (ja()) {
				proutn("Выделить энергию для щитов- ");
				action = NRG;
			}
			else if (damage[DSHIELD]) {
				prout("Щиты повреждены и отключены.");
				return;
			}
			else if (shldup) {
				proutn("Щиты подняты. Деактивировать? ");
				if (ja()) action = SHDN;
				else {
					chew();
					return;
				}
			}
			else {
				proutn("Щиты опущены. Активировать? ");
				if (ja()) action = SHUP;
				else {
					chew();
					return;
				}
			}
		}
	}
	switch (action) {
		case SHUP: /* raise shields */
			if (shldup) {
				prout("Щиты уже активированы.");
				return;
			}
			shldup = 1;
			shldchg = 1;
			if (condit != IHDOCKED) energy -= 50.0;
			prout("Щиты подняты.");
			if (energy <= 0) {
				skip(1);
				prout("Поднятие щитов израсходует остатки энергии.");
				finish(FNRG);
				return;
			}
			ididit=1;
			return;
		case SHDN:
			if (shldup==0) {
				prout("Щиты уже деактивированы.");
				return;
			}
			shldup=0;
			shldchg=1;
			prout("Защита понижена.");
			ididit=1;
			return;
		case NRG:
			while (scan() != IHREAL) {
				chew();
				proutn("Выделить энергию для щитов- ");
			}
			chew();
			if (aaitem==0) return;
			if (aaitem > energy) {
				prout("Нехватка энергии.");
				return;
			}
			ididit = 1;
			if (shield+aaitem >= inshld) {
				prout("Щиты на максимальной мощности.");
				if (shield+aaitem > inshld) {
					prout("Запрошенная избыточная энергия возвращена в энергоустановку");
				}
				energy -= inshld-shield;
				shield = inshld;
				return;
			}
			if (aaitem < 0.0 && energy-aaitem > inenrg) {
				/* Prevent shield drain loophole */
				skip(1);
				prout("Инженер мостику--");
				prout("  Здесь Скотт. Проблемы с энергетическим циклом, Капитан.");
				prout("  Невозможно отвести энергию от щитов.");
				ididit = 0;
				return;
			}
			if (shield+aaitem < 0) {
				prout("Вся энергия со щитов переводится на корабль.");
				energy += shield;
				shield = 0.0;
				return;
			}
			proutn("Скотти- \"");
			if (aaitem > 0)
				prout("Переправляю энергию на щиты.\"");
			else
				prout("Отвожу энергию от щитов.\"");
			shield += aaitem;
			energy -= aaitem;
			return;
		case NONE: break;
	}
}

void ram(int ibumpd, int ienm, int ix, int iy) {
	double type = 1.0, extradm;
	int icas, l;

	prouts("***ТРЕВОГА!  ТРЕВОГА!");
	skip(1);
	prout("***УГРОЗА СТОЛКНОВЕНИЯ.");
	skip(2);
	proutn("***");
	crmshp();
	switch (ienm) {
		case IHR: type = 1.5; break;
		case IHC: type = 2.0; break;
		case IHS: type = 2.5; break;
		case IHT: type = 0.5; break;
	}
	proutn(ibumpd ? " протаранен " : " тараны ");
	crmena(0, ienm, 2, ix, iy);
	if (ibumpd) proutn(" (исходная позиция)");
	skip(1);
	deadkl(ix, iy, ienm, sectx, secty);
	proutn("***");
	crmshp();
	prout(" сильно поврежден.");
	icas = 10.0+20.0*Rand();
	proutn("***отчет корабельного лазарета ");
	crami(icas, 1);
	prout(" жертвы.");
	casual += icas;
	for (l=1; l <= ndevice; l++) {
		if (l == DDRAY) continue; // Don't damage deathray
		if (damage[l] < 0) continue;
		extradm = (10.0*type*Rand()+1.0)*damfac;
		damage[l] += Time + extradm; /* Damage for at least time of travel! */
	}
	shldup = 0;
	if (d.remkl) {
		pause(2);
		dreprt();
	}
	else finish(FWON);
	return;
}

void torpedo(double course, double r, int inx, int iny, double *hit) {
	int l, iquad, ix, iy,  jx, jy, shoved=0, ll;
	double ac=course + 0.25*r;
	double angle = (15.0-ac)*0.5235988;
	double bullseye = (15.0 - course)*0.5235988;
	double deltax=-sin(angle), deltay=cos(angle), x=inx, y=iny, bigger;
	double ang, temp, xx, yy, kp, h1;

	bigger = fabs(deltax);
	if (fabs(deltay) > bigger) bigger = fabs(deltay);
	deltax /= bigger;
	deltay /= bigger;

	/* Loop to move a single torpedo */
	for (l=1; l <= 15; l++) {
		x += deltax;
		ix = x + 0.5;
		if (ix < 1 || ix > 10) break;
		y += deltay;
		iy = y + 0.5;
		if (iy < 1 || iy > 10) break;
		if (l==4 || l==9) skip(1);
		cramf(x, 0, 1);
		proutn(" - ");
		cramf(y, 0, 1);
		proutn("   ");
		iquad=quad[ix][iy];
		if (iquad==IHDOT) continue;
		/* hit something */
		skip(1);
		switch(iquad) {
			case IHE: /* Hit our ship */
			case IHF:
				skip(1);
				proutn("Попаданий торпед ");
				crmshp();
				prout(".");
				*hit = 700.0 + 100.0*Rand() -
					   1000.0*sqrt(square(ix-inx)+square(iy-iny))*
					   fabs(sin(bullseye-angle));
				*hit = fabs(*hit);
				newcnd(); /* undock */
				/* We may be displaced. */
				if (landed==1) return; /* Cheat if on a planet */
				ang = angle + 2.5*(Rand()-0.5);
				temp = fabs(sin(ang));
				if (fabs(cos(ang)) > temp) temp = fabs(cos(ang));
				xx = -sin(ang)/temp;
				yy = cos(ang)/temp;
				jx=ix+xx+0.5;
				jy=iy+yy+0.5;
				if (jx<1 || jx>10 || jy<1 ||jy > 10) return;
				if (quad[jx][jy]==IHBLANK) {
					finish(FHOLE);
					return;
				}
				if (quad[jx][jy]!=IHDOT) {
					/* can't move into object */
					return;
				}
				sectx = jx;
				secty = jy;
				crmshp();
				shoved = 1;
				break;

			case IHC: /* Hit a commander */
			case IHS:
				if (Rand() <= 0.05) {
					crmena(1, iquad, 2, ix, iy);
					prout(" использовал анти-фотонник;");
					prout("   торпеда нейтрализована.");
					return;
				}
			case IHR: /* Hit a regular enemy */
			case IHK:
				/* find the enemy */
				for (ll=1; ll <= nenhere; ll++)
					if (ix==kx[ll] && iy==ky[ll]) break;
				kp = fabs(kpower[ll]);
				h1 = 700.0 + 100.0*Rand() -
					 1000.0*sqrt(square(ix-inx)+square(iy-iny))*
					 fabs(sin(bullseye-angle));
				h1 = fabs(h1);
				if (kp < h1) h1 = kp;
				kpower[ll] -= (kpower[ll]<0 ? -h1 : h1);
				if (kpower[ll] == 0) {
					deadkl(ix, iy, iquad, ix, iy);
					return;
				}
				crmena(1, iquad, 2, ix, iy);
				/* If enemy damaged but not destroyed, try to displace */
				ang = angle + 2.5*(Rand()-0.5);
				temp = fabs(sin(ang));
				if (fabs(cos(ang)) > temp) temp = fabs(cos(ang));
				xx = -sin(ang)/temp;
				yy = cos(ang)/temp;
				jx=ix+xx+0.5;
				jy=iy+yy+0.5;
				if (jx<1 || jx>10 || jy<1 ||jy > 10) {
					prout(" поврежден, но не уничтожен.");
					return;
				}
				if (quad[jx][jy]==IHBLANK) {
					prout(" протиснулся в черную дыру.");
					deadkl(ix, iy, iquad, jx, jy);
					return;
				}
				if (quad[jx][jy]!=IHDOT) {
					/* can't move into object */
					prout(" поврежден, но не уничтожен.");
					return;
				}
				prout(" поврежден--");
				kx[ll] = jx;
				ky[ll] = jy;
				shoved = 1;
				break;
			case IHB: /* Hit a base */
				prout("***ЗВЕЗДНАЯ БАЗА УНИЧТОЖЕНА...");
				if (starch[quadx][quady] < 0) starch[quadx][quady] = 0;
				for (ll=1; ll<=d.rembase; ll++) {
					if (d.baseqx[ll]==quadx && d.baseqy[ll]==quady) {
						d.baseqx[ll]=d.baseqx[d.rembase];
						d.baseqy[ll]=d.baseqy[d.rembase];
						break;
					}
				}
				quad[ix][iy]=IHDOT;
				d.rembase--;
				basex=basey=0;
				d.galaxy[quadx][quady] -= 10;
				d.basekl++;
				newcnd();
				return;
			case IHP: /* Hit a planet */
				crmena(1, iquad, 2, ix, iy);
				prout(" уничтожен.");
				d.nplankl++;
				d.newstuf[quadx][quady] -= 1;
				d.plnets[iplnet] = nulplanet;
				iplnet = 0;
				plnetx = plnety = 0;
				quad[ix][iy] = IHDOT;
				if (landed==1) {
					/* captain parishes on planet */
					finish(FDPLANET);
				}
				return;
			case IHSTAR: /* Hit a star */
				if (Rand() > 0.10) {
					nova(ix, iy);
					return;
				}
				crmena(1, IHSTAR, 2, ix, iy);
				prout(" фотонный бластер не эффективен.");
				return;
			case IHQUEST: /* Hit a thingy */
				skip(1);
				prouts("ААААСУКАБЛЯЯЯЯЯ!!!");
				skip(1);
				prouts("    РУБИ!     ХУЯРЬ!    ПИЗДЯЧЬ!        *ДУШИ!*  ");
				skip(1);
				proutn("Спок-");
				prouts("  \"Очаровательно!\"");
				skip(1);
				quad[ix][iy] = IHDOT;
				return;
			case IHBLANK: /* Black hole */
				skip(1);
				crmena(1, IHBLANK, 2, ix, iy);
				prout(" проглотил торпеду.");
				return;
			case IHWEB: /* hit the web */
				skip(1);
				prout("***торпеда поглощена толианской сетью.");
				return;
			case IHT:  /* Hit a Tholian */
				skip(1);
				crmena(1, IHT, 2, ix, iy);
				h1 = 700.0 + 100.0*Rand() -
					 1000.0*sqrt(square(ix-inx)+square(iy-iny))*
					 fabs(sin(bullseye-angle));
				h1 = fabs(h1);
				if (h1 >= 600) {
					prout(" уничтожен.");
					quad[ix][iy] = IHDOT;
					ithere = 0;
					ithx = ithy = 0;
					return;
				}
				if (Rand() > 0.05) {
					prout(" выдержал удар фотонного бластера.");
					return;
				}
				prout(" исчез.");
				quad[ix][iy] = IHWEB;
				ithere = ithx = ithy = 0;
				{
					int dum, my;
					dropin(IHBLANK, &dum, &my);
				}
				return;

			default: /* Problem! */
				skip(1);
				proutn("Не знаю, как выдержать столкновение с ");
				crmena(1, iquad, 2, ix, iy);
				skip(1);
				return;
		}
		break;
	}
	if (shoved) {
		quad[jx][jy]=iquad;
		quad[ix][iy]=IHDOT;
		proutn(" сдвинут взрывом на");
		cramlc(2, jx, jy);
		skip(1);
		for (ll=1; ll<=nenhere; ll++)
			kdist[ll] = kavgd[ll] = sqrt(square(sectx-kx[ll])+square(secty-ky[ll]));
		sortkl();
		return;
	}
	skip(1);
	prout("Торпеда промазала.");
	return;
}

static void fry(double hit) {
	double ncrit, extradm;
	int ktr=1, l, ll, j, cdam[6], crptr;

	/* a critical hit occured */
	if (hit < (275.0-25.0*skill)*(1.0+0.5*Rand())) return;

	ncrit = 1.0 + hit/(500.0+100.0*Rand());
	proutn("***КРИТИЧЕСКОЕ ПОВРЕЖДЕНИЕ--");
	/* Select devices and cause damage */
	for (l = 1; l <= ncrit; l++) {
		do {
			j = ndevice*Rand()+1.0;
			/* Cheat to prevent shuttle damage unless on ship */
		} while (damage[j] < 0.0 || (j == DSHUTTL && iscraft != 1) ||
#ifdef CLOAKING
				 (j == DCLOAK && ship != IHE) ||
#endif
				 j == DDRAY);
		cdam[l] = j;
		extradm = (hit*damfac)/(ncrit*(75.0+25.0*Rand()));
		damage[j] += extradm;
		if (l > 1) {
			for (ll=2; ll<=l && j != cdam[ll-1]; ll++) ;
			if (ll<=l) continue;
			ktr += 1;
			if (ktr==3) skip(1);
			proutn(" и ");
		}
		proutn(device[j]);
	}
	prout(" поврежден.");
	if (damage[DSHIELD] && shldup) {
		prout("***Щиты сбиты.");
		shldup=0;
	}
#ifdef CLOAKING
	if (damage[DCLOAK] && iscloaked)
	{
		prout("***Маскировочный модуль не функционирует.");
		iscloaked = FALSE;
	}
#endif
}

void attack(int k) {
	/* k == 0 forces use of phasers in an attack */
	int percent, ihurt=0, l, i=0, jx, jy, iquad, itflag;
	int atackd = 0, attempt = 0;
	double hit;
	double pfac, dustfac, hitmax=0.0, hittot=0.0, chgfac=1.0, r;

#ifdef CLOAKING
    if (iscloaked && !iscloaking) return; // Nothing happens if we are cloaked
#endif

	iattak = 1;
	if (alldone) return;
#ifdef DEBUG
	if (idebug) prout("АТАКА!");
#endif

	if (ithere) movetho();

	if (neutz) { /* The one chance not to be attacked */
		neutz = 0;
		return;
	}
	if (((comhere || ishere) && (justin == 0)) || skill == SEMERITUS) movcom();
	if (nenhere==0) return;
	pfac = 1.0/inshld;
	if (shldchg == 1) chgfac = 0.25+0.5*Rand();
	skip(1);
	if (skill <= SFAIR) i = 2;
	for (l=1; l <= nenhere; l++) {
		if (kpower[l] < 0) continue;	/* too weak to attack */
		/* compute hit strength and diminsh shield power */
		r = Rand();
		/* Increase chance of photon torpedos if docked or enemy energy low */
		if (condit == IHDOCKED) r *= 0.25;
		if (kpower[l] < 500) r *= 0.25;
		jx = kx[l];
		jy = ky[l];
		iquad = quad[jx][jy];
		itflag = (iquad == IHK && r > 0.0005) || k == 0 ||
			(iquad==IHC && r > 0.015) ||
			(iquad==IHR && r > 0.3) ||
			(iquad==IHS && r > 0.07);
		if (itflag) {
			/* Enemy uses phasers */
			if (condit == IHDOCKED) continue; /* Don't waste the effort! */
			attempt = 1; /* Attempt to attack */
			dustfac = 0.8+0.05*Rand();
			hit = kpower[l]*pow(dustfac,kavgd[l]);
			kpower[l] *= 0.75;
		}
		else { /* Enemy used photon torpedo */
			double course = 1.90985*atan2((double)secty-jy, (double)jx-sectx);
			hit = 0;
			proutn("***ТОРПЕДА ПРИБЛИЖАЕТСЯ");
			if (damage[DSRSENS] <= 0.0) {
				proutn(" От ");
				crmena(0, iquad, i, jx, jy);
			}
			attempt = 1;
			prout("--");
			r = (Rand()+Rand())*0.5 -0.5;
			r += 0.002*kpower[l]*r;
			torpedo(course, r, jx, jy, &hit);
			if (d.remkl==0) finish(FWON); /* Klingons did themselves in! */
			if (d.galaxy[quadx][quady] == 1000 ||
				alldone) return; /* Supernova or finished */
			if (hit == 0) continue;
		}
		if (shldup != 0 || shldchg != 0) {
			/* shields will take hits */
			double absorb, hitsh, propor = pfac*shield;
			if(propor < 0.1) propor = 0.1;
			hitsh = propor*chgfac*hit+1.0;
			atackd=1;
			absorb = 0.8*hitsh;
			if (absorb > shield) absorb = shield;
			shield -= absorb;
			hit -= hitsh;
			if (propor > 0.1 && hit < 0.005*energy) continue;
		}
		/* It's a hit -- print out hit size */
		atackd = 1; /* We weren't going to check casualties, etc. if
		               shields were down for some strange reason. This
					   doesn't make any sense, so I've fixed it */
		ihurt = 1;
		cramf(hit, 0, 2);
		proutn(" единица повреждения");
		if ((damage[DSRSENS] > 0 && itflag) || skill <= SFAIR) {
			proutn(" на ");
			crmshp();
		}
		if (damage[DSRSENS] <= 0.0 && itflag) {
			proutn(" от ");
			crmena(0, iquad, i, jx, jy);
		}
		skip(1);
		/* Decide if hit is critical */
		if (hit > hitmax) hitmax = hit;
		hittot += hit;
		fry(hit);
		printf("Попадание %g энергия %g\n", hit, energy);
		energy -= hit;
	}
	if (energy <= 0) {
		/* Returning home upon your shield, not with it... */
		finish(FBATTLE);
		return;
	}
	if (attempt == 0 && condit == IHDOCKED)
		prout("***Враги решили атаковать ваш корабль.");
	if (atackd == 0) return;
	percent = 100.0*pfac*shield+0.5;
	if (ihurt==0) {
		/* Shields fully protect ship */
		proutn("Вражеская атака понизила мощность щитов на ");
	}
	else {
		/* Print message if starship suffered hit(s) */
		skip(1);
		proutn("Осталось энергии ");
		cramf(energy, 0, 2);
		proutn("    щиты ");
		if (shldup) proutn("подняты, ");
		else if (damage[DSHIELD] == 0) proutn("опущены, ");
		else proutn("повреждены, ");
	}
	crami(percent, 1);
	proutn("%   топред осталось ");
	crami(torps, 1);
	skip(1);
	/* Check if anyone was hurt */
	if (hitmax >= 200 || hittot >= 500) {
		int icas= hittot*Rand()*0.015;
		if (icas >= 2) {
			skip(1);
			proutn("МакКой-  \"Лазарет мостику.  У нас ");
			crami(icas, 1);
			prout(" пострадавших");
			prout("   в последней атаке.\"");
			casual += icas;
		}
	}
	/* After attack, reset average distance to enemies */
	for (l = 1; l <= nenhere; l++)
		kavgd[l] = kdist[l];
	sortkl();
	return;
}

void deadkl(int ix, int iy, int type, int ixx, int iyy) {
	/* Added ixx and iyy allow enemy to "move" before dying */

	int i,j;

	crmena(1, type, 2, ixx, iyy);
	/* Decide what kind of enemy it is and update approriately */
	if (type == IHR) {
		/* chalk up a Romulan */
		d.newstuf[quadx][quady] -= 10;
		irhere--;
		d.nromkl++;
		d.nromrem--;
	}
	else if (type == IHT) {
		/* Killed a Tholean */
		ithere = 0;
	}
	else {
		/* Some type of a Klingon */
		d.galaxy[quadx][quady] -= 100;
		klhere--;
		d.remkl--;
		switch (type) {
			case IHC:
				comhere = 0;
				for (i=1; i<=d.remcom; i++)
					if (d.cx[i]==quadx && d.cy[i]==quady) break;
				d.cx[i] = d.cx[d.remcom];
				d.cy[i] = d.cy[d.remcom];
				d.cx[d.remcom] = 0;
				d.cy[d.remcom] = 0;
				d.remcom--;
				future[FTBEAM] = 1e30;
				if (d.remcom != 0)
					future[FTBEAM] = d.date + expran(1.0*incom/d.remcom);
				d.killc++;
				break;
			case IHK:
				d.killk++;
				break;
			case IHS:
				d.nscrem = ishere = d.isx = d.isy = isatb = iscate = 0;
				d.nsckill = 1;
				future[FSCMOVE] = future[FSCDBAS] = 1e30;
				break;
		}
	}

	/* For each kind of enemy, finish message to player */
	prout(" уничтожен.");
	quad[ix][iy] = IHDOT;
	if (d.remkl==0) return;

	d.remtime = d.remres/(d.remkl + 4*d.remcom);

	if (type == IHT) return;

	/* Remove enemy ship from arrays describing local conditions */

	for (i=1; i<=nenhere; i++)
		if (kx[i]==ix && ky[i]==iy) break;
	nenhere--;
	if (i <= nenhere)  {
		for (j=i; j<=nenhere; j++) {
			kx[j] = kx[j+1];
			ky[j] = ky[j+1];
			kpower[j] = kpower[j+1];
			kavgd[j] = kdist[j] = kdist[j+1];
		}
	}
	kx[nenhere+1] = 0;
	ky[nenhere+1] = 0;
	kdist[nenhere+1] = 0;
	kavgd[nenhere+1] = 0;
	kpower[nenhere+1] = 0;
	return;
}

static int targetcheck(double x, double y, double *course) {
	double deltx, delty;
	/* Return TRUE if target is invalid */
	if (x < 1.0 || x > 10.0 || y < 1.0 || y > 10.0) {
		huh();
		return 1;
	}
	deltx = 0.1*(y - secty);
	delty = 0.1*(sectx - x);
	if (deltx==0 && delty== 0) {
		skip(1);
		prout("Спок-  \"Мостик лазарету.  Др. МакКой,");
		prout("  я рекомендую немедленно проверить");
		prout("  психологический профиль капитана.");
		chew();
		return 1;
	}
	*course = 1.90985932*atan2(deltx, delty);
	return 0;
}

void photon(void) {
	double targ[4][3], course[4];
	double r, dummy;
	int key, n, i, osuabor;

	ididit = 0;

	if (damage[DPHOTON]) {
		prout("Торпедные аппараты повреждены.");
		chew();
		return;
	}
	if (torps == 0) {
		prout("Торпеды закончились.");
		chew();
		return;
	}
	key = scan();
	for (;;) {
		if (key == IHALPHA) {
			huh();
			return;
		}
		else if (key == IHEOL) {
			crami(torps,1);
			prout(" торпед осталось.");
			proutn("Количество торпед в залпе- ");
			key = scan();
		}
		else /* key == IHREAL */ {
			n = aaitem + 0.5;
			if (n <= 0) { /* abort command */
				chew();
				return;
			}
			if (n > 3) {
				prout("Максимум 3 торпеды в залпе.");
			} else if (n <= torps) break;
			chew();
			key = IHEOL;
		}
	}
	for (i = 1; i <= n; i++) {
		key = scan();
		if (i==1 && key == IHEOL) {
			break;	/* we will try prompting */
		}
		if (i==2 && key == IHEOL) {
			/* direct all torpedoes at one target */
			while (i <= n) {
				targ[i][1] = targ[1][1];
				targ[i][2] = targ[1][2];
				course[i] = course[1];
				i++;
			}
			break;
		}
		if (key != IHREAL) {
			huh();
			return;
		}
		targ[i][1] = aaitem;
		key = scan();
		if (key != IHREAL) {
			huh();
			return;
		}
		targ[i][2] = aaitem;
		if (targetcheck(targ[i][1], targ[i][2], &course[i])) return;
	}
	chew();
	if (i == 1 && key == IHEOL) {
		/* prompt for each one */
		for (i = 1; i <= n; i++) {
			proutn("Целевой сектор для торпеды номер ");
			crami(i, 2);
			proutn("- ");
			key = scan();
			if (key != IHREAL) {
				huh();
				return;
			}
			targ[i][1] = aaitem;
			key = scan();
			if (key != IHREAL) {
				huh();
				return;
			}
			targ[i][2] = aaitem;
			chew();
			if (targetcheck(targ[i][1], targ[i][2], &course[i])) return;
		}
	}
	ididit = 1;
	/* Loop for moving <n> torpedoes */
	osuabor = 0;
	for (i = 1; i <= n && !osuabor; i++) {
		if (condit != IHDOCKED) torps--;
		r = (Rand()+Rand())*0.5 -0.5;
		if (fabs(r) >= 0.47) {
			/* misfire! */
			r = (Rand()+1.2) * r;
			if (n>1) {
				prouts("***ТОРПЕДА НОМЕР");
				crami(i, 2);
				prouts(" ОСЕЧКА.");
			}
			else prouts("***СБОЙ ТОРПЕДЫ.");
			skip(1);
			if (i < n)
				prout("  залп отменен.");
			osuabor=1;
			if (Rand() <= 0.2) {
				prout("***Торпедный аппарат поврежден.");
				damage[DPHOTON] = damfac*(1.0+2.0*Rand());
				break;
			}
		}
#ifdef CLOAKING
		if (iscloaked) r *= 1.2; /* Torpedoes are less accurate */
		else
#endif
		if (shldup != 0 || condit == IHDOCKED) r *= 1.0 + 0.0001*shield; /* Torpedos are less accurate */

		if (n != 1) {
			skip(1);
			proutn("След торпеды номер ");
			crami(i, 2);
			proutn("-   ");
		}
		else {
			skip(1);
			proutn("След торпеды- ");
		}
		torpedo(course[i], r, sectx, secty, &dummy);
		if (alldone || d.galaxy[quadx][quady]==1000) return;
	}
	if (d.remkl==0) finish(FWON);
}



static void overheat(double rpow) {
	if (rpow > 1500) {
		double chekbrn = (rpow-1500.)*0.00038;
		if (Rand() <= chekbrn) {
			prout("Оружейник Сулу-  \"фазеры перегреты, сэр.\"");
			damage[DPHASER] = damfac*(1.0 + Rand()) * (1.0+chekbrn);
		}
	}
}

static int checkshctrl(double rpow) {
	double hit;
	int icas;

	skip(1);
	if (Rand() < .998) {
		prout("Щиты опущены.");
		return 0;
	}
	/* Something bad has happened */
	prouts("***ТРЕВОГА!  ТРЕВОГА!");
	skip(2);
	hit = rpow*shield/inshld;
	energy -= rpow+hit*0.8;
	shield -= hit*0.2;
	if (energy <= 0.0) {
		prouts("Сулу-  \"Капитан! Щиты повре*************\"");
		skip(1);
		stars();
		finish(FPHASER);
		return 1;
	}
	prouts("Сулу-  \"Капитан! Щиты повреждены! Фазер горит!\"");
	skip(2);
	prout("Лт. Ухура-  \"Сэр, все отсеки сообщают о повреждениях.\"");
	icas = hit*Rand()*0.012;
	skip(1);
	fry(0.8*hit);
	if (icas) {
		skip(1);
		prout("МакКой мостику- \"Сильные радиационные ожоги, Джим.");
		proutn("  ");
		crami(icas, 1);
		prout(" пострадавших. Пока...\"");
		casual += icas; // Changed from -=, October 2013
	}
	skip(1);
	prout("Удар фазера поглощен щитами.");
	prout("Враг неповрежден.");
	overheat(rpow);
	return 1;
}


void phasers(void) {
	double hits[21], rpow, extra, powrem, over, temp;
	int kz = 0, k=1, i; /* Cheating inhibitor */
	int ifast=0, no=0, ipoop=1, msgflag = 1;
	enum {NOTSET, MANUAL, FORCEMAN, AUTOMATIC} automode = NOTSET;
	int key;

	skip(1);
	/* SR sensors and Computer */
	if (damage[DSRSENS]+damage[DCOMPTR] > 0) ipoop = 0;
	if (condit == IHDOCKED) {
		prout("Фазеры не могут стрелять через щиты базы.");
		chew();
		return;
	}
	if (damage[DPHASER] != 0) {
		prout("Контроль фазера поврежден.");
		chew();
		return;
	}
	if (shldup) {
		if (damage[DSHCTRL]) {
			prout("Контроль щита поврежден.");
			chew();
			return;
		}
		if (energy <= 200.0) {
			prout("Недостаточно энергии для активации контроля высокоскоростного щита.");
			chew();
			return;
		}
		prout("Оружейник Сулу-  \"Контроль высокоскоростного щита включен, сэр.\"");
		ifast = 1;

	}
	ididit = 1;
	/* Original code so convoluted, I re-did it all */
	while (automode==NOTSET) {
		key=scan();
		if (key == IHALPHA) {
			if (isit("manual")) {
				if (nenhere==0) {
					prout("Врагов не обнаружено.");
					chew();
					key = IHEOL;
					automode=AUTOMATIC;
				}
				else {
					automode = MANUAL;
					key = scan();
				}
			}
			else if (isit("automatic")) {
				if ((!ipoop) && nenhere != 0) {
					automode = FORCEMAN;
				}
				else {
					if (nenhere==0)
						prout("Энергия будет сброшена в космос.");
					automode = AUTOMATIC;
					key = scan();
				}
			}
			else if (isit("no")) {
				no = 1;
			}
			else {
				huh();
				ididit = 0;
				return;
			}
		}
		else if (key == IHREAL) {
			if (nenhere==0) {
				prout("Энергия будет сброшена в космос.");
				automode = AUTOMATIC;
			}
			else if (!ipoop)
				automode = FORCEMAN;
			else
				automode = AUTOMATIC;
		}
		else {
			/* IHEOL */
			if (nenhere==0) {
				prout("Энергия будет сброшена в космос.");
				automode = AUTOMATIC;
			}
			else if (!ipoop)
				automode = FORCEMAN;
			else
			proutn("Ручной или автоматический режим? ");
		}
	}

	switch (automode) {
		case AUTOMATIC:
			if (key == IHALPHA && isit("no")) {
				no = 1;
				key = scan();
			}
			if (key != IHREAL && nenhere != 0) {
				proutn("Фазеры захватили цель. Доступная энергия =");
				cramf(ifast?energy-200.0:energy,1,2);
				skip(1);
			}
			do {
				while (key != IHREAL) {
					chew();
					proutn("Единиц в залпе=");
					key = scan();
				}
				rpow = aaitem;
				if (rpow >= (ifast?energy-200:energy)) {
					proutn("Доступная энергия= ");
					cramf(ifast?energy-200:energy, 1,2);
					skip(1);
					key = IHEOL;
				}
			} while (rpow >= (ifast?energy-200:energy));
			if (rpow<=0) {
				/* chicken out */
				ididit = 0;
				chew();
				return;
			}
			if ((key=scan()) == IHALPHA && isit("no")) {
				no = 1;
			}
			if (ifast) {
				energy -= 200; /* Go and do it! */
				if (checkshctrl(rpow)) return;
			}
			chew();
			energy -= rpow;
			extra = rpow;
			if (nenhere) {
				extra = 0.0;
				powrem = rpow;
				for (i = 1; i <= nenhere; i++) {
					hits[i] = 0.0;
					if (powrem <= 0) continue;
					hits[i] = fabs(kpower[i])/(phasefac*pow(0.90,kdist[i]));
					over = (0.01 + 0.05*Rand())*hits[i];
					temp = powrem;
					powrem -= hits[i] + over;
					if (powrem <= 0 && temp < hits[i]) hits[i] = temp;
					if (powrem <= 0) over = 0.0;
					extra += over;
				}
				if (powrem > 0.0) extra += powrem;
				hittem(hits);
			}
			if (extra > 0 && alldone == 0) {
				if (ithere) {
					proutn("*** Толианская сеть поглощает ");
					if (nenhere>0) proutn("избыточную ");
					prout("энергию фазеров.");
				}
				else {
					cramf(extra, 0, 2);
					prout(" сброшено в открытый космос.");
				}
			}
			break;

		case FORCEMAN:
			chew();
			key = IHEOL;
			if (damage[DCOMPTR]!=0)
				prout("Боевой компьютер поврежден, переход на ручное управление.");
			else {
				skip(1);
				prouts("---ОБРАБОТКА---");
				skip(1);
				prout("Близкодействующие-сенсоры-повреждены");
				prout("Недостаточно-информации-для-автоматического-огня-фазеров");
				prout("Необходим-переход-на ручное-управление");
				skip(1);
			}
		case MANUAL:
			rpow = 0.0;
			for (k = 1; k <= nenhere;) {
				int ii = kx[k], jj = ky[k];
				int ienm = quad[ii][jj];
				if (msgflag) {
					proutn("Доступно энергии= ");
					cramf(energy-.006-(ifast?200:0), 0, 2);
					skip(1);
					msgflag = 0;
					rpow = 0.0;
				}
				if (damage[DSRSENS] && !(abs(sectx-ii) < 2 && abs(secty-jj) < 2) &&
					(ienm == IHC || ienm == IHS)) {
					cramen(ienm);
					prout(" не может быть обнаружен без близкодействуюшего сканера.");
					chew();
					key = IHEOL;
					hits[k] = 0; /* prevent overflow -- thanks to Alexei Voitenko */
					k++;
					continue;
				}
				if (key == IHEOL) {
					chew();
					if (ipoop && k > kz) {
						int irec=(fabs(kpower[k])/(phasefac*pow(0.9,kdist[k])))*
								 (1.01+0.05*Rand()) + 1.0;
						kz = k;
						proutn("(");
						crami(irec, 1);
						proutn(")  ");
					}
					proutn("Единиц к залпу по ");
					crmena(0, ienm, 2, ii, jj);
					proutn("-  ");
					key = scan();
				}
				if (key == IHALPHA && isit("no")) {
					no = 1;
					key = scan();
					continue;
					}
				if (key == IHALPHA) {
					huh();
					ididit = 0;
					return;
				}
				if (key == IHEOL) {
					if (k==1) { /* Let me say I'm baffled by this */
						msgflag = 1;
					}
					continue;
				}
				if (aaitem < 0) {
					/* abort out */
					ididit = 0;
					chew();
					return;
				}
				hits[k] = aaitem;
				rpow += aaitem;
				/* If total requested is too much, inform and start over */

				if (rpow >= (ifast?energy-200:energy)) {
					prout("Превышение доступной энергии -- попробуйте снова.");
					chew();
					key = IHEOL;
					k = 1;
					msgflag = 1;
					continue;
				}
				key = scan(); /* scan for next value */
				k++;
			}
			if (rpow == 0.0) {
				/* zero energy -- abort */
				ididit = 0;
				chew();
				return;
			}
			if (key == IHALPHA && isit("no")) {
				no = 1;
			}
			energy -= rpow;
			chew();
			if (ifast) {
				energy -= 200.0;
				if (checkshctrl(rpow)) return;
			}
			hittem(hits);
			ididit=1;
			break;
			case NOTSET: break; // cannot occur
	}
	/* Say shield raised or malfunction, if necessary */
	if (alldone) return;
	if (ifast) {
		skip(1);
		if (no == 0) {
			if (Rand() >= 0.99) {
				prout("Сулу-  \"Сэр, контроль высокоскоросного щита поврежден . . .");
				prouts("         ЩЕЛК   ШЕЛК   ЧПОК  . . .");
				prout(" Нет ответа, сэр!");
				shldup = 0;
			}
			else
				prout("Щиты активны.");
		}
		else
			shldup = 0;
	}
	overheat(rpow);
}

void hittem(double *hits) {
	double kp, kpow, wham, hit, dustfac, kpini;
	int nenhr2=nenhere, k=1, kk=1, ii, jj, ienm;

	skip(1);

	for (; k <= nenhr2; k++, kk++) {
		if ((wham = hits[k])==0) continue;
		dustfac = 0.9 + 0.01*Rand();
		hit = wham*pow(dustfac,kdist[kk]);
		kpini = kpower[kk];
		kp = fabs(kpini);
		if (phasefac*hit < kp) kp = phasefac*hit;
		kpower[kk] -= (kpower[kk] < 0 ? -kp: kp);
		kpow = kpower[kk];
		ii = kx[kk];
		jj = ky[kk];
		if (hit > 0.005) {
			cramf(hit, 0, 2);
			proutn(" единиц повреждения ");
		}
		else
			proutn("Небольшие повреждения ");
		ienm = quad[ii][jj];
		crmena(0,ienm,2,ii,jj);
		skip(1);
		if (kpow == 0) {
			deadkl(ii, jj, ienm, ii, jj);
			if (d.remkl==0) finish(FWON);
			if (alldone) return;
			kk--; /* don't do the increment */
		}
		else /* decide whether or not to emasculate klingon */
			if (kpow > 0 && Rand() >= 0.9 &&
				kpow <= ((0.4 + 0.4*Rand())*kpini)) {
				proutn("***Спок-  \"Капитан, судно в квадрате");
				cramlc(2,ii,jj);
				skip(1);
				prout("   потеряло огневую мощь.\"");
				kpower[kk] = -kpow;
			}
	}
	return;
}

#ifdef CAPTURE
/*	$NetBSD: capture.c,v 1.6 2003/08/07 09:37:50 agc Exp $	*/

/*
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
**  Ask a Klingon To Surrender
**
**	(Fat chance)
**
**	The Subspace Radio is needed to ask a Klingon if he will kindly
**	surrender.  A random Klingon from the ones in the quadrant is
**	chosen.
**
**	The Klingon is requested to surrender.  The probability of this
**	is a function of that Klingon's remaining power, our power,
**	etc.
*/

int selectklingon(void);

void
   capture(void)
{
	int		i;
	int k;
	double			x;

	ididit = FALSE; // Nothing if we fail
	Time = 0.0;

	/* Make sure there is room in the brig */
	if (brigfree == 0)
	{
		printf("Служба безопасности сообщает, что карцер полон.\n");
		return;
	}

	if (!REPORTS) {
		printf("Ухура- \"Мы потеряли подпространственную связь, сэр.\"\n");
		return;
	}

	if (damage[DTRANSP] != 0) {
		printf("Скотти- \"Транспортер поврежден, сэр.\"\n");
		return;
	}



	/* find out if there are any at all */
	if (klhere < 1)
	{
		printf("Ухура- \"Нет ответа, сэр.\"\n");
		return;
	}

	/* if there is more than one Klingon, find out which one */
	k = selectklingon();
	Time = 0.05;   // This action will take some time
	ididit = TRUE; //  So any others can strike back

    /* check out that Klingon */
    /* The algorithm isn't that great and could use some more
     * intelligent design */
//	x = 300 + 25*skill;
	x = energy;
	x /= kpower[k] * nenhere;
	x *= 2.5;  /* would originally have been equivalent of 1.4, but we want command to work more often, more humanely */
	i = x;
#ifdef DEBUG
	printf("Prob = %d (%.4f)\n", i, x);
//	i = 100; // For testing, of course!
#endif
	if (i > 100*Rand())
	{
		/* guess what, he surrendered!!! */
		printf("Клингонский капитан в квадрате %d,%d выживших\n", kx[k], ky[k]);
		i = 200*Rand();
		if ( i > 0 )
			printf("%d Клингоны совершили самоубийство, предпочтя его плену\n", 200 - i);
		if (i > brigfree)
		{
			printf("%d клингонов погибло из-за отсутствия места в карцере.\n", i-brigfree);
			i = brigfree;
		}
		brigfree -= i;
		printf("%d пленных взято\n", i);
		deadkl(kx[k], ky[k], quad[kx[k]][ky[k]], kx[k], ky[k]);
		if (d.remkl==0) finish(FWON);
		return;
	}

	/* big surprise, he refuses to surrender */
	printf("Жирный шанс, капитан\n");
	return;
}


/*
 **  SELECT A KLINGON
 **
 **	Cruddy, just takes one at random.  Should ask the captain.
 **	Nah, just select the weakest one since it is most likely to
 **	surrender (Tom Almy mod)
 */

int selectklingon()
{
	int		i;

	if (nenhere < 2)
		i = 1;
	else
	{	// Select the weakest one
		double pow  = 1e6;
		int j;
		for (j=1; j <= nenhere; j++)
		{
			if (quad[kx[j]][ky[j]] == IHR) continue; // No Romulans surrender
			if (kpower[j]< pow)
			{
				pow = kpower[j];
				i = j;
			}
		}
	}
	return i;
}

#endif
