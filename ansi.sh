bgbla='\033[40m'; bgred='\033[41m'; bggre='\033[42m'; bgbro='\033[43m';
bgblu='\033[44m'; bgmag='\033[45m'; bgcya='\033[46m'; bggra='\033[47m';
bla='\033[30m'; red='\033[31m'; gre='\033[32m'; bro='\033[33m';
blu='\033[34m'; mag='\033[35m'; cya='\033[36m'; gra='\033[37m';
bbla='\033[30;1m'; bred='\033[31;1m'; bgre='\033[32;1m'; yel='\033[33;1m';
bblu='\033[34;1m'; bmag='\033[35;1m'; bcya='\033[36;1m'; whi='\033[37;1m';
rst='\033[0m'; inv='\033[7m'; cll='\033[2K'; cllr='\033[2K';
cls='\033[2J'; clsb='\033[J'; 

bgblae='[40m'; bgrede='[41m'; bggree='[42m'; bgbroe='[43m';
bgblue='[44m'; bgmage='[45m'; bgcyae='[46m'; bggrae='[47m';
blae='[30m'; rede='[31m'; gree='[32m'; broe='[33m';
blue='[34m'; mage='[35m'; cyae='[36m'; grae='[37m';
bblae='[30;1m'; brede='[31;1m'; bgree='[32;1m'; yele='[33;1m';
bblue='[34;1m'; bmage='[35;1m'; bcyae='[36;1m'; whie='[37;1m';
rste='[0m'; inve='[7m'; clle='[2K';
cllre='[2K'; clse='[2J'; clsbe='[J';

a24fg () { # [0-255] Usage: a24fg r g b
	printf '%s' "[38;2;$1;$2;${3}m"
}
a24bg () { # [0-255] Usage: a24fg r g b
	printf '%s' "[48;2;$1;$2;${3}m"
}
