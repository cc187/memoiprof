

#include "MemoiProfiler.h"
#include "cJSON.h"
#include "zf_log.h"

#include <glib.h>
#include <stdio.h>
#include <math.h>

void profiler_test();

void hash_table_test();

void multi_out_test();

void ComputeQCPU_extract(float expArg, float phi, float *o0, float *o1);

int main() {

//    hash_table_test();

//    profiler_test();

    multi_out_test();

    return 0;
}

void multi_out_test() {

    int keys[] = {15, 8, 14, 7, 7, 12, 3, 12, 22, 10, 18, 14, 25, 18, 26, 24, 29, 23, 20, 13, 1, 5, 3, 9, 21, 14, 29,
                  26, 24, 20, 15, 13, 10, 2, 28, 27, 22, 20, 23, 16, 21, 7, 20, 25, 15, 1, 15, 10, 21, 5, 4, 14, 6, 28,
                  21, 9, 29, 27, 22, 20, 28, 9, 29, 27, 17, 14, 9, 19, 18, 24, 26, 7, 6, 25, 10, 2, 6, 5, 29, 5, 1, 17,
                  29, 22, 19, 6, 5, 11, 24, 6, 1, 20, 13, 27, 15, 2, 5, 14, 25, 8};

    float keys_f[] = {0.02479103817513547266f, 0.60099397839365368763f, 0.14037311350809674572f,
                      0.02039664317158467504f, 0.90982638581143259729f, 0.55683724214937991979f,
                      0.18056868836273318060f, 0.68955538595474185736f, 0.14551106204712824076f,
                      0.56533264161036308642f, 0.55738735155320751237f, 0.79690362492307405783f,
                      0.33011867092532270680f, 0.43754479923656275020f, 0.65123424658175503929f,
                      0.60607573483154537111f, 0.03789426186556336946f, 0.64934983933878277882f,
                      0.18037417488866684208f, 0.52007882723442398152f, 0.36729761189970664014f,
                      0.88368555441886501902f, 0.33629173230441764121f, 0.78393851325097650473f,
                      0.50536976786374188685f, 0.72978214334168066250f, 0.71693357962112013383f,
                      0.14496542449908192451f, 0.05557574582903331356f, 0.79593583545096398278f,
                      0.81433463189516922028f, 0.04557629517748806153f, 0.73517548105913042080f,
                      0.64533507376055474062f, 0.15590574701956569544f, 0.89528209147478885204f,
                      0.65670479938200403876f, 0.61618709181824222952f, 0.08548451221986949944f,
                      0.72976495983289278314f, 0.37137481685966015184f, 0.27795490199045703378f,
                      0.29089975068033356093f, 0.48447849228621526525f, 0.71050696233763874095f,
                      0.33563013159280198850f, 0.26935587428366609307f, 0.86220335329256583015f,
                      0.27802538157451456004f, 0.75121929999804182778f, 0.81323726647731042423f,
                      0.06442055923363376900f, 0.64035459686212321730f, 0.83564554542996305997f,
                      0.55637342613722920051f, 0.64895387342318842779f, 0.35038448642288784267f,
                      0.31715828424227006543f, 0.06893295811384012085f, 0.06832178721200649571f,
                      0.68435546132384034162f, 0.75717257594081654430f, 0.98761399717714542498f,
                      0.07615130095430130147f, 0.44176913902139118000f, 0.17467870157440209792f,
                      0.49437864730021499100f, 0.76891071233411449848f, 0.97216998686683577338f,
                      0.41510763514900320932f, 0.93013909861881365301f, 0.44706535759128985692f,
                      0.27112279039602483304f, 0.38484601371265167833f, 0.76380686374831303462f,
                      0.11491802669188463863f, 0.58420037194911944548f, 0.55887254515534192805f,
                      0.88843880254560072531f, 0.27113197967234207141f, 0.28471076618692780089f,
                      0.02902990090472859730f, 0.65874871223776228323f, 0.75755179682455454418f,
                      0.02859434275481697942f, 0.90199786834991495622f, 0.33133715878091522872f,
                      0.64439462249675957186f, 0.75073966241226016656f, 0.32697248464849517758f,
                      0.29163583108573701265f, 0.28419387898879494014f, 0.90039803441195903887f,
                      0.81659071884961875922f, 0.01373342749022718336f, 0.71670965706413908635f,
                      0.58066926087682414195f, 0.98661994464403388865f, 0.35905455529033551759f,
                      0.23935243412665627659f, 0.25111921441840986607f, 0.99626326662884380767f,
                      0.30090079783541718782f, 0.74646996625977412314f, 0.05427994126112432373f,
                      0.66219353006272373302f, 0.10488913686604449659f, 0.54517820317160466907f,
                      0.54772962017308780741f, 0.22403520639057910772f, 0.71897728847743435830f,
                      0.95049967699031674211f, 0.50447202859008674585f, 0.29285999075344426979f,
                      0.38690077609715493309f, 0.12207677424801468084f, 0.43565761642258173754f,
                      0.06241154176856588608f, 0.03995490457295379226f, 0.51967941222572496324f,
                      0.68948996793231791594f, 0.96706563787603076004f, 0.34131552043345629858f,
                      0.71030972029882181860f, 0.09769459303758213097f, 0.01243273504933894872f,
                      0.46925546889600068524f, 0.80181356314020253567f, 0.89842658926223985554f,
                      0.10867260850989870134f, 0.21297747444086598587f, 0.14051321629862132293f,
                      0.36213970793086265818f, 0.24375642660712060723f, 0.68924525653052569098f,
                      0.30130078279681260475f, 0.39352343876569662520f, 0.25858838395188247384f,
                      0.38964963063668151664f, 0.34420004955020897625f, 0.14652558392725677908f,
                      0.57188179156744765512f, 0.90006166777494103267f, 0.72844065796493461542f,
                      0.93305642667543478018f, 0.31128842364406337547f, 0.87140003878146313170f,
                      0.27007002060293249963f, 0.54379045310862955396f, 0.74658217778267272234f,
                      0.64426992757487723210f, 0.75011810537817605871f, 0.35213342070919550650f,
                      0.02689523973633772337f, 0.46659785431119992976f, 0.58388115962593267749f,
                      0.75409446900778978855f, 0.86292904855152221027f, 0.62847058126243363924f,
                      0.53584787927074090214f, 0.21237761343583887890f, 0.62754318389104316231f,
                      0.99625151346538361385f, 0.31026143509929038477f, 0.17950816835766697295f,
                      0.88159871412966016775f, 0.41487620183853356188f, 0.55196633437797554706f,
                      0.45910106745553814278f, 0.42737738681161734695f, 0.38151838336384313172f,
                      0.99239877921423526550f, 0.66120098296607933807f, 0.07489445359228539490f,
                      0.32484925265072907001f, 0.60014941481843125467f, 0.84684756048045738688f,
                      0.03582905070414200858f, 0.84802486633434675181f, 0.14161011750684846004f,
                      0.69788167814959629342f, 0.56927689798376879199f, 0.97803500375266536276f,
                      0.66516070898960136865f, 0.46916253127232313117f, 0.98661336666458388462f,
                      0.03275031140029273181f, 0.11459441137788781080f, 0.28898318241111596985f,
                      0.68029999223755137120f, 0.57730666848618982451f, 0.73221864019777816615f,
                      0.67637844870420050040f, 0.02471077033662407932f, 0.82317300178788740832f,
                      0.12446733530388329361f, 0.82584114991289999552f, 0.09903179633244363163f,
                      0.62278070730282701401f, 0.99305027513308257130f, 0.92888819400642865980f,
                      0.55860284414930130894f, 0.34059571125353988292f, 0.91068982390069305466f,
                      0.02981111108323554763f, 0.47851570207428749325f, 0.93775517759069657001f,
                      0.99310027757486718632f, 0.09108201365390140681f, 0.31036107517295097490f,
                      0.37877992447995628958f, 0.55565746602059622921f, 0.59471848982727124746f,
                      0.05740061144839137896f, 0.72721499724565302782f, 0.64445519588286341530f,
                      0.65494929885933586509f, 0.41250455312611500383f, 0.15268593529361342943f,
                      0.68743330552532078513f, 0.70812476257986895322f, 0.42964663545753851375f,
                      0.98414786670729424157f, 0.58983164084564027331f, 0.58247685259492987940f,
                      0.08561057149429730193f, 0.77309529425342440832f, 0.56905745363125035355f,
                      0.18705885911071405203f, 0.82089733677786004921f, 0.08265407445608526095f,
                      0.07957345172621381125f, 0.53503926069954144445f, 0.39156962318869409630f,
                      0.77230877426354504513f, 0.61290727609109097972f, 0.08971333314038673161f,
                      0.97774944764074699889f, 0.40576895067451322606f, 0.10154760629214507389f,
                      0.88084805518151151969f, 0.41280213867611389288f, 0.04758998717583018478f,
                      0.92933836689439241878f, 0.20534094343123203565f, 0.71025165961061160579f,
                      0.72891983297884861967f, 0.20819791512737267829f, 0.47206588626438761280f,
                      0.82449343312976452126f, 0.96093634996386546974f, 0.72152288580685853499f,
                      0.12761311746414657057f, 0.41559898620821994104f, 0.52663295206186503857f,
                      0.97479125907378497402f, 0.46959265051012925547f, 0.97504457147127912659f,
                      0.12493070965289192965f, 0.51495167224755527942f, 0.03227331931596153137f,
                      0.97347405130958819203f, 0.04698626923981731860f, 0.72589564367932764788f,
                      0.38609463147951771430f, 0.98761064789007364923f, 0.91850046326757741335f,
                      0.30517972786814113181f, 0.78046267286980817003f, 0.72995843210055732647f,
                      0.43271301045676464766f, 0.21297922947444366176f, 0.76904441837182224765f,
                      0.67395822116017261532f, 0.92605974533274965448f, 0.56606959194701332007f,
                      0.35935057875299293500f, 0.58318939179910121842f, 0.70681732418280557445f,
                      0.28360967624474612034f, 0.35564414067847423725f, 0.23919132281258734355f,
                      0.42565961160258032757f, 0.01452318043886016398f, 0.37509763092480924337f,
                      0.98073184598505284182f, 0.62861173598941302038f, 0.47232502508153876153f,
                      0.39548327568303723855f, 0.37569389470412707217f, 0.19950773003167450064f,
                      0.02763626038840599898f, 0.32167420489700606480f, 0.50689295596396614144f,
                      0.68063140612883178108f, 0.27155421786031557892f, 0.92916643504978520112f,
                      0.45895632171756031107f, 0.81645242657937025797f, 0.05414512394528211993f,
                      0.49194472969002092375f, 0.92630660840534555379f, 0.75303449223836216053f,
                      0.62134049633208980571f, 0.27646335077503742194f, 0.30391068994707080915f,
                      0.21298641526649262251f, 0.51332418784546428300f, 0.46359287330888033216f,
                      0.87833217178859762346f, 0.52577347396819129269f, 0.99595756384602045998f,
                      0.09674173956138522870f, 0.70923805031446968630f, 0.95623169503304026129f,
                      0.39588861560679282931f, 0.53650148045847159289f, 0.20282088124571047771f,
                      0.97662675313013821057f, 0.37716428600339355863f, 0.50573820662803754973f,
                      0.03352308371814465260f, 0.20021546898011644045f, 0.75942623927967119758f,
                      0.13825873601843427270f, 0.09740034440162913287f, 0.40664561524548833529f,
                      0.01635219616912091322f, 0.80475138094162947487f, 0.85442855042291280618f,
                      0.98716178636618713087f, 0.98111130989116132264f, 0.45397915034167387855f,
                      0.10223824621614873682f, 0.38869225847467812739f, 0.85578723704276658747f,
                      0.38743333663383372716f, 0.66623220961854384059f, 0.80585349660409422775f,
                      0.18102375205927572089f, 0.24551770633117595673f, 0.00201241697712405535f,
                      0.18954372223152000372f, 0.98719813883292740280f, 0.37714182935822088276f,
                      0.16024147048229280384f, 0.41846178258802294125f, 0.13103241193578956906f,
                      0.80417210487305413465f, 0.76702185678003337940f, 0.86865279038148202240f,
                      0.19198767212011932143f, 0.85138727770242386798f, 0.47552238243533375257f,
                      0.32568602345869079144f, 0.74734186715305656636f, 0.39267976708767228159f,
                      0.01246684025813024094f, 0.04680335527660301828f, 0.35197227057900601151f,
                      0.91043299467782903357f, 0.27065878249767687281f, 0.51368791377906236577f,
                      0.46423546731239590246f, 0.38249127187613832502f, 0.24874192230429901960f,
                      0.99640979944458011114f, 0.95572348166492010704f, 0.67212875542577239392f,
                      0.74203450379799331169f, 0.98743727795673095276f, 0.82657668468445176820f,
                      0.01482411875433927377f, 0.20696585278818289415f, 0.07580546371709425831f,
                      0.58673119052660071593f, 0.21968323796270871941f, 0.53646142962378105356f,
                      0.78568408876350503906f, 0.12030773529905319013f, 0.95488839594714800900f,
                      0.73175159575379483071f, 0.33503556683115111047f, 0.90623474954613307848f,
                      0.95145867213340208347f, 0.93315914613526932503f, 0.05707035858384464419f,
                      0.67055993002799432256f, 0.72427380781560037180f, 0.25794313140349282034f,
                      0.66750707678979740430f, 0.67909016783718878894f, 0.49194641994787013643f,
                      0.50179515187291173041f, 0.89686209496472628450f, 0.38873687751550002520f,
                      0.38680042606116127257f, 0.48841697831110324086f, 0.74319344788331283445f,
                      0.99567239648549727914f, 0.49374939375198823787f, 0.68861621806647455622f,
                      0.74500377894859660241f, 0.66460921795901837510f, 0.13326790526345170112f,
                      0.48535835622687109175f, 0.10820782453639349861f, 0.81925918219347391349f,
                      0.47758194680374780854f, 0.50140215782102411626f, 0.64115494909731070286f,
                      0.09040713895490850305f, 0.47400095283271149669f, 0.47612438267986929604f,
                      0.74869989926819643193f, 0.50646256643213287536f, 0.91841590576006972103f,
                      0.93371073019208520490f, 0.59995203749928044408f, 0.83951955992165399485f,
                      0.59936299072444981874f, 0.27382778558639023675f, 0.45848586088147021104f,
                      0.52155048358624481571f, 0.74050568116046791334f, 0.87570443988045828247f,
                      0.34055644038677598226f, 0.80622264245178843818f, 0.23325118840346265384f,
                      0.31531072285825822005f, 0.36271450631204562105f, 0.69065967568300805266f,
                      0.14100451598559266168f, 0.37386762561320132101f, 0.14580226825265908717f,
                      0.89922872973245015632f, 0.22040296263411111193f, 0.51956400094633172562f,
                      0.32407937696408962394f, 0.32533554791124021853f, 0.42801337582134552966f,
                      0.51437489571939690068f, 0.92814819397859113412f, 0.19729055946803562674f,
                      0.21476450821214158403f, 0.82429826812260744729f, 0.35276005431874994193f,
                      0.70614212459899976870f, 0.59883080357077427347f, 0.43626205995497979108f,
                      0.90644851647995540325f, 0.76709943296060037378f, 0.64602761871134261164f,
                      0.30008533252804095493f, 0.01354807046433188850f, 0.82750439514873587795f,
                      0.87179221605002685704f, 0.35596305990487056299f, 0.33269735229665890073f,
                      0.22907664022532368062f, 0.19723955142470412986f, 0.90830593241370803397f,
                      0.40871031136221884204f, 0.36045896859342227211f, 0.06861670189524148370f,
                      0.14667975186529991226f, 0.36635375302656878892f, 0.72810893042681632738f,
                      0.31583540416779931722f, 0.00818275308701665523f, 0.84925553534874413801f,
                      0.71817397526904748206f, 0.90732440359888914772f, 0.31538788949375432360f,
                      0.26715712058714209152f, 0.32121074194805675201f, 0.42261427521777047024f,
                      0.66218949763253320293f, 0.11913259071745692722f, 0.35296680885292596591f,
                      0.76711271092067769309f, 0.97249012842104847185f, 0.68482722651584728156f,
                      0.14617079935042513105f, 0.72458860876312696055f, 0.21777068385659058521f,
                      0.50563009286819081825f, 0.58981258461401713160f, 0.65999260761781170944f,
                      0.52163384539124255601f, 0.63022542308578926888f, 0.66065395327836678719f,
                      0.29318700302251469586f, 0.56929319113595453619f, 0.72499703635685622708f,
                      0.78127585429871047592f, 0.20865224217403524740f, 0.59593014343448896995f,
                      0.00190602392133630167f, 0.13703143988423393586f, 0.94787678764310022362f,
                      0.32533108804394625541f, 0.10691158062132127958f, 0.30959690190550628812f,
                      0.41646528852396871896f, 0.26371293154450295835f, 0.02562894438161043142f,
                      0.98218881800972230577f, 0.46958268617286207799f, 0.90744746847117728467f,
                      0.64166711359927380453f, 0.20505486443345883435f, 0.29441640622526571822f,
                      0.86620817877243760796f, 0.09558969162416187302f, 0.91846789323720547595f,
                      0.20303373061152866601f, 0.83116883270681656059f, 0.79379502783275574765f,
                      0.09911004762018653902f, 0.63922558516070115628f, 0.99401730538962673338f,
                      0.75000324276934670967f, 0.93677001627643536811f, 0.24177205554836618009f,
                      0.08673146933102271659f, 0.69866912678163949038f, 0.05668726783829044442f,
                      0.26890195341739169106f, 0.78989348819385293000f, 0.07114052835093300733f,
                      0.11166893746587959439f, 0.65367139918540823090f, 0.75963155165088213241f,
                      0.09966913810085725382f, 0.73509897382212738555f, 0.10371405524374938270f,
                      0.58521301766249031708f, 0.64253039999527433484f, 0.63684369762667486610f,
                      0.83597427618698670141f, 0.89086401998182717768f, 0.62366686260860381722f,
                      0.47442170556828605095f, 0.31769804501559810649f, 0.38185328045659498326f,
                      0.74569079462886107261f, 0.86136532380203816464f, 0.18389949571009616708f,
                      0.03217280487129309827f, 0.07458711426347000326f, 0.34383822143125427084f,
                      0.45180353965843951073f, 0.97131329343004420745f, 0.02332466354445973553f,
                      0.99677550917858811497f, 0.12526087992491594500f, 0.35221895171870222082f,
                      0.27452302793012256180f, 0.63441365873634946354f, 0.61194101701266333026f,
                      0.55020436156921891563f, 0.53287162452681579469f, 0.66470912449935189904f,
                      0.37632653903092026799f, 0.23177401090363403967f, 0.07158762136681388031f,
                      0.88649146505185764069f, 0.64675761070078504417f, 0.20463898390285541433f,
                      0.49351382622625443027f, 0.95993918084284340433f, 0.26286135618344892513f,
                      0.23270212130866400543f, 0.01619629879260734385f, 0.59621020149912615162f,
                      0.02060922480353641850f, 0.25924874418793411397f, 0.21774927232783761590f,
                      0.08716499506395847074f, 0.55686228562424775232f, 0.33832794488124116844f,
                      0.69858506703995928022f, 0.09231144016395048553f, 0.28872687311850606496f,
                      0.87890473348113069586f, 0.41401446628723818837f, 0.06111627498041281179f,
                      0.94105388477300576690f, 0.33158928984702175015f, 0.81730422367673928812f,
                      0.09244533301820888492f, 0.05491941508643722165f, 0.53245762162354916576f,
                      0.71049116536044466121f, 0.30811585222704622127f, 0.79860389568259148497f,
                      0.52925356294920941334f, 0.41713916330661306119f, 0.61483266456986035696f,
                      0.71134775879149728288f, 0.93613494805586548880f, 0.24903457596191377666f,
                      0.83781720633550206678f, 0.61373125644199853078f, 0.40268193813402701597f,
                      0.94406380908226652166f, 0.37410832710665685738f, 0.41032027769943866808f,
                      0.50699868860629417415f, 0.80879469593938164463f, 0.01748492659759328967f,
                      0.55747579016912565334f, 0.25908917595620210792f, 0.67568156704233061840f,
                      0.09622437764142504807f, 0.11282118995224666475f, 0.36558672642070973887f,
                      0.41753594357447681074f, 0.37618128656164112087f, 0.01731875470458413901f,
                      0.43174278513000051518f, 0.94951732412413879002f, 0.75747643023585287260f,
                      0.13499801447622495868f, 0.31616031774402143359f, 0.51442774693768018907f,
                      0.88084004017797277181f, 0.71927139126278280363f, 0.32550666928897974010f,
                      0.77095489924003584959f, 0.34059711009438649875f, 0.74141644695919259945f,
                      0.32605799317683055656f, 0.80962567145720015699f, 0.20531677642965081718f,
                      0.51644520900392315756f, 0.63821638685697936907f, 0.69329770909236597304f,
                      0.06139432720001531378f, 0.90808474028190971939f, 0.70655280917456084942f,
                      0.79190308154329754505f, 0.36118724324704578022f, 0.71574411006452376557f,
                      0.14665864497640724476f, 0.20645427009241881279f, 0.98515353919737863435f,
                      0.18633830144561420539f, 0.37443059223421081956f, 0.98452990026128284748f,
                      0.44895975255227728360f, 0.29759496825530974164f, 0.66470714400291430415f,
                      0.41212029966713270432f, 0.00359273592681304230f, 0.77184914362105912199f,
                      0.01273087808163270357f, 0.36465837809480729869f, 0.91106175442051284844f,
                      0.37295696720196789570f, 0.48582741777768588054f, 0.06372240471496094018f,
                      0.22644681927072085406f, 0.92740000706783795865f, 0.23641689079704594798f,
                      0.83924248234637592170f, 0.46735446297423193253f, 0.59108140679138530232f,
                      0.59160928954650522286f, 0.98528759932670994953f, 0.06992147276113247821f,
                      0.03362796966044235466f, 0.70451782357169079934f, 0.23032129092949627182f,
                      0.17537584701487291661f, 0.09034280762122059132f, 0.73959410569269269080f,
                      0.09941150316542897836f, 0.12333308018560994868f, 0.23338670316684355893f,
                      0.13840385763764112662f, 0.88676199842769489730f, 0.95813721902715722641f,
                      0.18024979527099399874f, 0.30318152176980463541f, 0.71099724070890754044f,
                      0.19480792320642386031f, 0.86407055368738566080f, 0.59490441481111285160f,
                      0.20757696682305730465f, 0.05463988676864749704f, 0.64216881646676199475f,
                      0.67766618990612417573f, 0.63856495903344337188f, 0.75467131433618181145f,
                      0.30604958540911826479f, 0.86051681120851977947f, 0.39485313936867844683f,
                      0.48602797723542587695f, 0.31414817172569676352f, 0.84552080738820411989f,
                      0.48307278432296383417f, 0.34463434678665489367f, 0.86588831728292372124f,
                      0.04971983791412280077f, 0.19860099783737470476f, 0.00850354907221572106f,
                      0.50054330320838261174f, 0.00771244592176790264f, 0.09934156683822979784f,
                      0.31768045699477500090f, 0.39989304471438662209f, 0.53324499954914191936f,
                      0.80306723198959978938f, 0.67959386104803339606f, 0.61698088095316162143f,
                      0.92553593817817072508f, 0.87355632011988818098f, 0.65276804630369625181f,
                      0.35478694600074696335f, 0.87620918401532535856f, 0.99419599379222605071f,
                      0.21865873423093772487f, 0.49982612989452638249f, 0.47498107779344214676f,
                      0.86484290928940955665f, 0.43720989830711339806f, 0.42361279885946383963f,
                      0.19716398939721682917f, 0.96213810627210863937f, 0.23781117374902268564f,
                      0.56590912650182345607f, 0.76790021063877579152f, 0.16681617207130349616f,
                      0.49088421672529541113f, 0.37636918843806905540f, 0.28819545475978793610f,
                      0.96511415509633912895f, 0.35861471432673593192f, 0.57243338210996586745f,
                      0.50870238925669279436f, 0.11499170456775658537f, 0.05446596895643685374f,
                      0.83553798135062078467f, 0.80640021635979319077f, 0.85914696887572110464f,
                      0.72445785791289098287f, 0.82655093534061867358f, 0.73809759956491139739f,
                      0.41835812422202757650f, 0.51399722641503719444f, 0.38498708709254263455f,
                      0.37360502548692948060f, 0.36450537043034355408f, 0.27401440867808989038f,
                      0.88491030343512716056f, 0.02725146028754795697f, 0.41377132660827843931f,
                      0.71847526305443135421f, 0.99337314881148275081f, 0.93913920242247855321f,
                      0.42838712108773939580f, 0.63749806804923314277f, 0.76009942600307213434f,
                      0.67048140804341592105f, 0.88207457464898329734f, 0.29205628399835601465f,
                      0.96907243492144603870f, 0.77522857838088948350f, 0.52449696514112810972f,
                      0.91185131639791549748f, 0.72418977479568423515f, 0.45540978640671222687f,
                      0.66100335611047254999f, 0.01580018650729425772f, 0.36114337544372916732f,
                      0.59723347122930569930f, 0.24026724331526905449f, 0.49270275907795208105f,
                      0.45226377730191839500f, 0.28210125954305594450f, 0.84948217230278793148f,
                      0.07551440885122251456f, 0.01698729966581493968f, 0.50072624442532338071f,
                      0.07395709654551725516f, 0.54091098079392061975f, 0.55469093533624035132f,
                      0.13061579991209019163f, 0.33487657142909971269f, 0.42472354718845560323f,
                      0.80576222558065962583f, 0.02093908701310945895f, 0.28875910444587030217f,
                      0.36140540041860057565f, 0.64718347696900510345f, 0.41000922306051316091f,
                      0.33256792538396978365f, 0.73139689125003741490f, 0.13517005070617747476f,
                      0.11539430632091079709f, 0.36468242826350801779f, 0.97072232841061925835f,
                      0.02033088429817849268f, 0.26174054671701969071f, 0.33281854749169310606f,
                      0.23163518752081388067f, 0.93210246185466781504f, 0.81906636893987284456f,
                      0.37122238810366680334f, 0.55598267719911293336f, 0.54896370669219588851f,
                      0.01317644036862085131f, 0.16066956621096555706f, 0.79627262328278683840f,
                      0.15423674206973598421f, 0.89241478306811772402f, 0.41397299186053721003f,
                      0.78662046040944695632f, 0.35028766435299276621f, 0.26095558681461868347f,
                      0.44118953353088340072f, 0.62860912322980390354f, 0.65010965382592080093f,
                      0.71924679733917301970f, 0.66013007710664192740f, 0.40886440664772851568f,
                      0.82826472504453674946f, 0.90556525656749704837f, 0.40747778367447645131f,
                      0.04655854359899469906f, 0.87268774450337012688f, 0.75390530361234936370f,
                      0.75923738895283885931f, 0.71209685995450468983f, 0.91320356241770027701f,
                      0.19502997738874875362f, 0.98268325634514314820f, 0.68324566853046071076f,
                      0.58613197540278677717f, 0.71029133087834123406f, 0.32931632093534462504f,
                      0.60268369125417619144f, 0.23037445958889128466f, 0.91752919500184379255f,
                      0.63296569020800603047f, 0.52938030488381406556f, 0.97266441714684002740f,
                      0.19517147525796846140f, 0.24579398197487387123f, 0.70713862186151061243f,
                      0.93714494066549707459f, 0.20184886870769049026f, 0.59037373495998980031f,
                      0.03585752317900780242f, 0.96187854595001666195f, 0.07413563612443660376f,
                      0.09611791012048058302f, 0.87228847313848383360f, 0.63170923540956364864f,
                      0.84916334080114344898f, 0.01679406839429245557f, 0.60627245068435272488f,
                      0.70465383961670908399f, 0.48100949456846618955f, 0.45701517301057346872f,
                      0.77041427016374197422f, 0.58125427762422679814f, 0.44736705494535961674f,
                      0.10162575728968074979f, 0.72399285879179985805f, 0.24709487418405750820f,
                      0.29511233733669326219f, 0.94172158957826108668f, 0.14304958126192750661f,
                      0.26807996308602978444f, 0.05903941948051938152f, 0.75092432066535390760f,
                      0.17758414939470012638f, 0.52339729401655626866f, 0.04145126888507027277f,
                      0.99659371731690044539f, 0.28806847999222230116f, 0.52160708301530020105f,
                      0.67900651829894249576f, 0.45927600239652748756f, 0.00542244785003185314f,
                      0.68053418007277837359f, 0.85889691735942054267f, 0.44393530034739559847f,
                      0.80216268883146847195f, 0.56771055682524520553f, 0.23178144725167381851f,
                      0.76831657499028564167f, 0.16065743420530513847f, 0.85651969152080254881f,
                      0.32413699726345561132f, 0.78064655017245211327f, 0.63295107236259620274f,
                      0.88283513042044726036f, 0.20967473978838760454f, 0.05948228674938227823f,
                      0.67232310699815023121f, 0.01755073718091313116f, 0.15569162954056288447f,
                      0.61678393579983608947f, 0.78642435937493895396f, 0.61826032860708575368f,
                      0.70353867701917237760f, 0.12692192479084084823f, 0.28367290332040081255f,
                      0.78420937869310420892f, 0.91887937230757923633f, 0.64278235655829378750f,
                      0.40910228509765527813f, 0.43446538791849334662f, 0.39925462329965928496f,
                      0.69494252462489557858f, 0.14583064318550401769f, 0.40509502221883149384f,
                      0.91454686208885523304f, 0.12671360363390363549f, 0.69949878041507152562f,
                      0.97303474122842147079f, 0.25980492409572069005f, 0.17582367379448193341f,
                      0.64482615034407105769f, 0.31445436747794217629f, 0.10969199594398833853f,
                      0.53955357368216512246f, 0.78746100907526877014f, 0.64719813858602798359f,
                      0.35597161180699849411f, 0.37517222652282387576f, 0.15618906454972989743f,
                      0.06561436742323268916f, 0.08576169283307914051f, 0.36899736519148437533f,
                      0.13883197730988517599f, 0.39674649816100459529f, 0.83512256308307162347f,
                      0.09728882898118598291f, 0.36498268471731460361f, 0.82998025428322116977f,
                      0.34691912083136067645f, 0.66262344803191997553f, 0.82958348136022317423f,
                      0.24512944879335104854f, 0.02649405207530932628f, 0.76833727201394136649f,
                      0.82394883538813637478f, 0.82766923169523066112f, 0.76335919942569009869f,
                      0.58894160064711562812f, 0.47112418163907675286f, 0.52626151075708551921f,
                      0.14952922709027327940f, 0.64609637359351932973f, 0.30730321188932601904f,
                      0.77711045625800692143f, 0.94797425492159403969f, 0.08085901844808627632f,
                      0.37506944584075578070f, 0.95919123192985009710f, 0.39136968189664303301f,
                      0.42601946828098965125f, 0.00060179243082415893f, 0.68139300743591062645f,
                      0.23075966019621024334f, 0.18630297987727292235f, 0.20551974704471665902f,
                      0.11196219172100327174f, 0.32427314587128216712f, 0.24876911963048621096f,
                      0.92796053993280601815f, 0.10317633523068880853f, 0.13625686100684328365f,
                      0.12954047332461292102f, 0.81104738960470106189f, 0.16436689136133655184f,
                      0.51545659581161076919f, 0.82423077897805878084f, 0.13389979651117700686f,
                      0.96645106326212926302f, 0.44038905886098872202f, 0.33301317025491306397f,
                      0.76805716089080687766f, 0.55655855911025378019f, 0.51375597693983252195f,
                      0.74921379312877946435f, 0.97984077577100411986f, 0.00129126563298054519f,
                      0.25924463055403044201f, 0.70396319675855704756f, 0.19531609441538521392f,
                      0.17773212293258479937f, 0.28481740825267794271f, 0.32434673788180125211f,
                      0.49821712508099929009f};

    float keys_f2[] = {0.0137334167957f, 0.0137334177271f, 0.0137334186584f, 0.0137334195897f, 0.013733420521f,
                       0.0137334214523f, 0.0137334223837f, 0.013733423315f, 0.0137334242463f, 0.0137334251776f,
                       0.013733426109f, 0.0137334270403f, 0.0137334279716f, 0.0137334289029f, 0.0137334298342f,
                       0.0137334307656f};

    /*
    MemoiProf *mp_single = mp_init("cosf(float)", "multi_out", "mp_single.json", 1, 1, MP_FLOAT, MP_FLOAT);
    mp_set_culling(mp_single, 1);
    mp_set_sampling(mp_single, MP_SAMPLING_RANDOM, 2);

    for (int index = 0; index < 100; ++index) {

        float input = (float) keys[index];
        float output = cosf(input);
        mp_inc(mp_single, &input, &output);
    }
    mp_to_json(mp_single);

    mp_destroy(mp_single);
    /**/


    MemoiProf *mp_multi = mp_init("ComputeQCPU_extract(float,float,float*,float*)", "multi_out",
                                  "mp_multi.approx.json",
                                  2,
                                  2,
                                  MP_FLOAT, MP_FLOAT,
                                  MP_FLOAT, MP_FLOAT);
    mp_set_culling(mp_multi, MP_CULLING_OFF, 0.0f);
//    mp_set_periodic_reporting(mp_multi, MP_PERIODIC_ON, 20);
    mp_set_sampling(mp_multi, MP_SAMPLING_OFF, 20);
    mp_set_approx(mp_multi, MP_APPROX_ON, 4);

    for (int index = 0; index < 16; ++index) {

        float expArg = keys_f2[index];
        float phi = 1.0f; //rand() % (6 + 1 - 1) + 1;
        float o0, o1;
        ComputeQCPU_extract(expArg, phi, &o0, &o1);
        mp_inc(mp_multi, &expArg, &phi, &o0, &o1);
    }
    mp_to_json(mp_multi);

    mp_destroy(mp_multi);
}

void ComputeQCPU_extract(float expArg, float phi, float *o0, float *o1) {

    float cosArg = cosf(expArg);
    float sinArg = sinf(expArg);

    // outputs
    *o0 = cosArg * phi;
    *o1 = sinArg * phi;
}


void hash_table_test() {

    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    char *str = "hello";
    char *key = strdup(str);

    printf("start: %s\n", key);

    g_hash_table_insert(table, key, "world");
    printf("after 1st insert: %s\n", key);

    g_hash_table_insert(table, key, "world");
    printf("after 2nd insert: %s\n", key);

    g_hash_table_destroy(table);

    printf("end: %s\n", key);
}

void profiler_test() {


    printf("-= Memoization Profiler Test =-\n");

    MemoiProf *mp = mp_init("log", "test", "mp.json", 1, 1, MP_FLOAT, MP_FLOAT);

    float i1 = 3.14f;
    float o1 = 345.127f;
    float i2 = 1.0f;
    float o2 = 0.0f;

    mp_set_call_sites(mp, 2, "main.c:19", "main.c:22");

    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);

    mp_inc(mp, &o2, &i2);
    mp_inc(mp, &o2, &i2);
    mp_inc(mp, &o2, &i2);
    mp_inc(mp, &o2, &i2);

    mp_print(mp);

    mp_to_json(mp);
    mp = mp_destroy(mp);

    MemoiProf *mi2 = mp_init("log", "tested", "mi2.json", 1, 1, MP_DOUBLE, MP_DOUBLE);

    double i12 = 3.14;
    double o12 = 345.127;

    mp_inc(mi2, &o12, &i12);
    mp_inc(mi2, &o12, &i12);
    mp_inc(mi2, &o12, &i12);
    mp_inc(mi2, &o12, &i12);

    mp_to_json(mi2);
    mi2 = mp_destroy(mi2);


    MemoiProf *mp_pow = mp_init("pow(double,double)", "2 args", "mp_pow.json", 2, 1, MP_DOUBLE, MP_DOUBLE,
                                MP_DOUBLE);
    mp_set_call_sites(mp_pow, 1, "global");
    mp_set_sampling(mp_pow, MP_SAMPLING_RANDOM, 100);
    mp_set_periodic_reporting(mp_pow, MP_PERIODIC_ON, 100);

    double base = 2.0;
    double power = 8.0;
    double output = pow(base, power);


    srand(time(NULL));   // Initialization, should only be called once.

    for (int i = 0; i < 100000; ++i) {

        const double b = i * 54 + output;
        const double e = rand() % 10;
        double o = pow(b, e);
        mp_inc(mp_pow, &o, &b, &e);
    }
//    mp_print(mp_pow);
    mp_to_json(mp_pow);


    MemoiProf *mp_numbers = mp_init("cos(double)", "numbers", "mp_numbers.json", 1, 1, MP_DOUBLE, MP_DOUBLE);

    int keys[] = {15, 8, 14, 7, 7, 12, 3, 12, 22, 10, 18, 14, 25, 18, 26, 24, 29, 23, 20, 13, 1, 5, 3, 9, 21, 14, 29,
                  26, 24, 20, 15, 13, 10, 2, 28, 27, 22, 20, 23, 16, 21, 7, 20, 25, 15, 1, 15, 10, 21, 5, 4, 14, 6, 28,
                  21, 9, 29, 27, 22, 20, 28, 9, 29, 27, 17, 14, 9, 19, 18, 24, 26, 7, 6, 25, 10, 2, 6, 5, 29, 5, 1, 17,
                  29, 22, 19, 6, 5, 11, 24, 6, 1, 20, 13, 27, 15, 2, 5, 14, 25, 8};

    for (int index = 0; index < 100; ++index) {

        double input = keys[index];
        double output = cos(input);
        mp_inc(mp_numbers, &output, &input);
    }
    mp_set_culling(mp_numbers, 1, 0.0);
    mp_to_json(mp_numbers);


    mp_pow = mp_destroy(mp_pow);
}
/**/