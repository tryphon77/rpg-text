#include <genesis.h>


const u16 intro_english = 0x0;
const u16 intro_japanese = 0x327;
const u16 intro_french = 0x5C4;
const u16 intro2_english = 0x937;
const u16 intro2_japanese = 0x94E;
const u16 intro2_french = 0x957;

const u8 paseo_data[] = {
/* intro_english:
[if(past_intro, intro2)][load(SCENE_INTRO_1)]I've been having terrible nightmares...[pause]
In them, a young girl is fighting an eldritch demon.[wait]
I watch them battle, but I can't move or speak. All I can do is helplessly look on as the girl is attacked.[clear][load(SCENE_BLACK)]
As always, I wake up just as she's exhausted all her strength.[clear][load(SCENE_INTRO_2)]
I'm now in my bedroom, dimly lit by the early dawn. An indescribable grief consumes me. I desperately hold back the tears in my eyes...[clear]
This is the modern age under the auspices of the supercomputer, Mother Brain. This is not the time to dwell on the nonsense of dreams.[clear]
Such fanciful notions have been replaced by world encompassing digital data.[clear]
I open my window and take a breath of fresh air.[clear]
The crisp morning breeze courses through my body, washing away any ill feelings left from my nightmare.[close]*/
	0xFF, 0x0C, 0x00, 0x00, 0xFF, 0xFF, 0x09, 0x2F, 0xFF, 0x06, 0x01, 0x49, 0x27, 0x76, 0x65, 0x00,
	0x62, 0x65, 0x65, 0x6E, 0x00, 0x68, 0x61, 0x76, 0x69, 0x6E, 0x67, 0x00, 0x74, 0x65, 0x72, 0x72,
	0x69, 0x62, 0x6C, 0x65, 0x00, 0x6E, 0x69, 0x67, 0x68, 0x74, 0x6D, 0x61, 0x72, 0x65, 0x73, 0x2E,
	0x2E, 0x2E, 0xFF, 0x04, 0xFF, 0x00, 0x49, 0x6E, 0x00, 0x74, 0x68, 0x65, 0x6D, 0x2C, 0x00, 0x61,
	0x00, 0x79, 0x6F, 0x75, 0x6E, 0x67, 0x00, 0x67, 0x69, 0x72, 0x6C, 0x00, 0x69, 0x73, 0x00, 0x66,
	0x69, 0x67, 0x68, 0x74, 0x69, 0x6E, 0x67, 0x00, 0x61, 0x6E, 0x00, 0x65, 0x6C, 0x64, 0x72, 0x69,
	0x74, 0x63, 0x68, 0x00, 0x64, 0x65, 0x6D, 0x6F, 0x6E, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0x49, 0x00,
	0x77, 0x61, 0x74, 0x63, 0x68, 0x00, 0x74, 0x68, 0x65, 0x6D, 0x00, 0x62, 0x61, 0x74, 0x74, 0x6C,
	0x65, 0x2C, 0x00, 0x62, 0x75, 0x74, 0x00, 0x49, 0x00, 0x63, 0x61, 0x6E, 0x27, 0x74, 0x00, 0x6D,
	0x6F, 0x76, 0x65, 0x00, 0x6F, 0x72, 0x00, 0x73, 0x70, 0x65, 0x61, 0x6B, 0x2E, 0x00, 0x41, 0x6C,
	0x6C, 0x00, 0x49, 0x00, 0x63, 0x61, 0x6E, 0x00, 0x64, 0x6F, 0x00, 0x69, 0x73, 0x00, 0x68, 0x65,
	0x6C, 0x70, 0x6C, 0x65, 0x73, 0x73, 0x6C, 0x79, 0x00, 0x6C, 0x6F, 0x6F, 0x6B, 0x00, 0x6F, 0x6E,
	0x00, 0x61, 0x73, 0x00, 0x74, 0x68, 0x65, 0x00, 0x67, 0x69, 0x72, 0x6C, 0x00, 0x69, 0x73, 0x00,
	0x61, 0x74, 0x74, 0x61, 0x63, 0x6B, 0x65, 0x64, 0x2E, 0xFF, 0x05, 0xFF, 0x06, 0x00, 0xFF, 0x00,
	0x41, 0x73, 0x00, 0x61, 0x6C, 0x77, 0x61, 0x79, 0x73, 0x2C, 0x00, 0x49, 0x00, 0x77, 0x61, 0x6B,
	0x65, 0x00, 0x75, 0x70, 0x00, 0x6A, 0x75, 0x73, 0x74, 0x00, 0x61, 0x73, 0x00, 0x73, 0x68, 0x65,
	0x27, 0x73, 0x00, 0x65, 0x78, 0x68, 0x61, 0x75, 0x73, 0x74, 0x65, 0x64, 0x00, 0x61, 0x6C, 0x6C,
	0x00, 0x68, 0x65, 0x72, 0x00, 0x73, 0x74, 0x72, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x2E, 0xFF, 0x05,
	0xFF, 0x06, 0x02, 0xFF, 0x00, 0x49, 0x27, 0x6D, 0x00, 0x6E, 0x6F, 0x77, 0x00, 0x69, 0x6E, 0x00,
	0x6D, 0x79, 0x00, 0x62, 0x65, 0x64, 0x72, 0x6F, 0x6F, 0x6D, 0x2C, 0x00, 0x64, 0x69, 0x6D, 0x6C,
	0x79, 0x00, 0x6C, 0x69, 0x74, 0x00, 0x62, 0x79, 0x00, 0x74, 0x68, 0x65, 0x00, 0x65, 0x61, 0x72,
	0x6C, 0x79, 0x00, 0x64, 0x61, 0x77, 0x6E, 0x2E, 0x00, 0x41, 0x6E, 0x00, 0x69, 0x6E, 0x64, 0x65,
	0x73, 0x63, 0x72, 0x69, 0x62, 0x61, 0x62, 0x6C, 0x65, 0x00, 0x67, 0x72, 0x69, 0x65, 0x66, 0x00,
	0x63, 0x6F, 0x6E, 0x73, 0x75, 0x6D, 0x65, 0x73, 0x00, 0x6D, 0x65, 0x2E, 0x00, 0x49, 0x00, 0x64,
	0x65, 0x73, 0x70, 0x65, 0x72, 0x61, 0x74, 0x65, 0x6C, 0x79, 0x00, 0x68, 0x6F, 0x6C, 0x64, 0x00,
	0x62, 0x61, 0x63, 0x6B, 0x00, 0x74, 0x68, 0x65, 0x00, 0x74, 0x65, 0x61, 0x72, 0x73, 0x00, 0x69,
	0x6E, 0x00, 0x6D, 0x79, 0x00, 0x65, 0x79, 0x65, 0x73, 0x2E, 0x2E, 0x2E, 0xFF, 0x05, 0xFF, 0x00,
	0x54, 0x68, 0x69, 0x73, 0x00, 0x69, 0x73, 0x00, 0x74, 0x68, 0x65, 0x00, 0x6D, 0x6F, 0x64, 0x65,
	0x72, 0x6E, 0x00, 0x61, 0x67, 0x65, 0x00, 0x75, 0x6E, 0x64, 0x65, 0x72, 0x00, 0x74, 0x68, 0x65,
	0x00, 0x61, 0x75, 0x73, 0x70, 0x69, 0x63, 0x65, 0x73, 0x00, 0x6F, 0x66, 0x00, 0x74, 0x68, 0x65,
	0x00, 0x73, 0x75, 0x70, 0x65, 0x72, 0x63, 0x6F, 0x6D, 0x70, 0x75, 0x74, 0x65, 0x72, 0x2C, 0x00,
	0x4D, 0x6F, 0x74, 0x68, 0x65, 0x72, 0x00, 0x42, 0x72, 0x61, 0x69, 0x6E, 0x2E, 0x00, 0x54, 0x68,
	0x69, 0x73, 0x00, 0x69, 0x73, 0x00, 0x6E, 0x6F, 0x74, 0x00, 0x74, 0x68, 0x65, 0x00, 0x74, 0x69,
	0x6D, 0x65, 0x00, 0x74, 0x6F, 0x00, 0x64, 0x77, 0x65, 0x6C, 0x6C, 0x00, 0x6F, 0x6E, 0x00, 0x74,
	0x68, 0x65, 0x00, 0x6E, 0x6F, 0x6E, 0x73, 0x65, 0x6E, 0x73, 0x65, 0x00, 0x6F, 0x66, 0x00, 0x64,
	0x72, 0x65, 0x61, 0x6D, 0x73, 0x2E, 0xFF, 0x05, 0xFF, 0x00, 0x53, 0x75, 0x63, 0x68, 0x00, 0x66,
	0x61, 0x6E, 0x63, 0x69, 0x66, 0x75, 0x6C, 0x00, 0x6E, 0x6F, 0x74, 0x69, 0x6F, 0x6E, 0x73, 0x00,
	0x68, 0x61, 0x76, 0x65, 0x00, 0x62, 0x65, 0x65, 0x6E, 0x00, 0x72, 0x65, 0x70, 0x6C, 0x61, 0x63,
	0x65, 0x64, 0x00, 0x62, 0x79, 0x00, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x00, 0x65, 0x6E, 0x63, 0x6F,
	0x6D, 0x70, 0x61, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x00, 0x64, 0x69, 0x67, 0x69, 0x74, 0x61, 0x6C,
	0x00, 0x64, 0x61, 0x74, 0x61, 0x2E, 0xFF, 0x05, 0xFF, 0x00, 0x49, 0x00, 0x6F, 0x70, 0x65, 0x6E,
	0x00, 0x6D, 0x79, 0x00, 0x77, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x00, 0x61, 0x6E, 0x64, 0x00, 0x74,
	0x61, 0x6B, 0x65, 0x00, 0x61, 0x00, 0x62, 0x72, 0x65, 0x61, 0x74, 0x68, 0x00, 0x6F, 0x66, 0x00,
	0x66, 0x72, 0x65, 0x73, 0x68, 0x00, 0x61, 0x69, 0x72, 0x2E, 0xFF, 0x05, 0xFF, 0x00, 0x54, 0x68,
	0x65, 0x00, 0x63, 0x72, 0x69, 0x73, 0x70, 0x00, 0x6D, 0x6F, 0x72, 0x6E, 0x69, 0x6E, 0x67, 0x00,
	0x62, 0x72, 0x65, 0x65, 0x7A, 0x65, 0x00, 0x63, 0x6F, 0x75, 0x72, 0x73, 0x65, 0x73, 0x00, 0x74,
	0x68, 0x72, 0x6F, 0x75, 0x67, 0x68, 0x00, 0x6D, 0x79, 0x00, 0x62, 0x6F, 0x64, 0x79, 0x2C, 0x00,
	0x77, 0x61, 0x73, 0x68, 0x69, 0x6E, 0x67, 0x00, 0x61, 0x77, 0x61, 0x79, 0x00, 0x61, 0x6E, 0x79,
	0x00, 0x69, 0x6C, 0x6C, 0x00, 0x66, 0x65, 0x65, 0x6C, 0x69, 0x6E, 0x67, 0x73, 0x00, 0x6C, 0x65,
	0x66, 0x74, 0x00, 0x66, 0x72, 0x6F, 0x6D, 0x00, 0x6D, 0x79, 0x00, 0x6E, 0x69, 0x67, 0x68, 0x74,
	0x6D, 0x61, 0x72, 0x65, 0x2E, 0xFF, 0x02,

/* intro_japanese:
[load(SCENE_INTRO_1)]そのころ ぼくは まいばん わるいゆめに うなされていた。[pause]
ひとりの しょうじょが きょだいな あくまのように まがまがしい かいぶつと たたかっているのだ...[wait]
ぼくは ちかくで それを みているのだが うごくことも できず こえも だせず しょうじょが まものに いためつけられるのを ただ みていることしか できないのだった...[wait][load(SCENE_BLACK)]
そして しょうじょが ちから つきようと しているところで ぼくは めがさめる。[wait][load(SCENE_INTRO_2)]
よあけの ほのぐらい へやのなかで ぼくは いいようのない かなしみに とらえられ こみあげてくる なみだを ひっしに こらえるのだった...[wait]
ぼくの なまえは ユーシス。 モタビアの しゅと パセオで エージェントとして はたらいている。
ぼくは あたまをふり あたまから ゆめを おいだそうとした。[wait]
マザーブレインという きょだいな メインコンピューターが せかいを しはいする げんだいに おいて、ゆめなどという あいまいな ものに センチメンタルに なるなんて[wait]
ナンセンスいがいの なにものでもない。[wait]
よのなかの できごとは すべて デジタルな データに おきかえられるものなのだから...[wait]
ぼくは まどをあけ あさのくうきを すいこんだ。
すがすがしい くうきが からだのなかを とおりぬけ、ゆめに とらわれていた じぶんが あらいながされていくような きがした...[close]*/
	0xFF, 0x06, 0x01, 0xFF, 0x01, 0x01, 0x0F, 0x19, 0x0A, 0x2B, 0x00, 0x46, 0x08, 0x1A, 0x00, 0x1F,
	0x02, 0x42, 0x2E, 0x00, 0x2C, 0x29, 0x02, 0x25, 0x22, 0x16, 0x00, 0x03, 0x15, 0x0B, 0x2A, 0x13,
	0x02, 0x10, 0x4C, 0xFF, 0x04, 0xFF, 0x00, 0x1B, 0x14, 0x28, 0x19, 0x00, 0x0C, 0x32, 0x03, 0x39,
	0x32, 0x33, 0x00, 0x07, 0x32, 0x3D, 0x02, 0x15, 0x00, 0x01, 0x08, 0x1F, 0x19, 0x26, 0x03, 0x16,
	0x00, 0x1F, 0x33, 0x1F, 0x33, 0x0C, 0x02, 0x00, 0x06, 0x02, 0x44, 0x12, 0x14, 0x00, 0x10, 0x10,
	0x06, 0x2F, 0x13, 0x02, 0x29, 0x19, 0x3D, 0xFF, 0x01, 0x00, 0x2E, 0x2E, 0x2E, 0xFF, 0x03, 0xFF,
	0x00, 0xFF, 0x01, 0x01, 0x46, 0x08, 0x1A, 0x00, 0x11, 0x06, 0x08, 0x40, 0x00, 0x0F, 0x2A, 0x2D,
	0x00, 0x20, 0x13, 0x02, 0x29, 0x19, 0x3D, 0x33, 0x00, 0x03, 0x37, 0x08, 0x0A, 0x14, 0x23, 0x00,
	0x40, 0x07, 0x3A, 0x00, 0x0A, 0x04, 0x23, 0x00, 0x3D, 0x0E, 0x3A, 0x00, 0x0C, 0x32, 0x03, 0x39,
	0x32, 0x33, 0x00, 0x1F, 0x23, 0x19, 0x16, 0x00, 0x02, 0x10, 0x22, 0x12, 0x09, 0x27, 0x2A, 0x29,
	0x19, 0x2D, 0x00, 0x10, 0x3D, 0x00, 0x20, 0x13, 0x02, 0x29, 0x0A, 0x14, 0x0C, 0x06, 0x00, 0x40,
	0x07, 0x15, 0x02, 0x19, 0x3D, 0x2F, 0x10, 0xFF, 0x01, 0x00, 0x2E, 0x2E, 0x2E, 0xFF, 0x03, 0xFF,
	0x06, 0x00, 0xFF, 0x00, 0xFF, 0x01, 0x01, 0x0F, 0x0C, 0x13, 0x00, 0x0C, 0x32, 0x03, 0x39, 0x32,
	0x33, 0x00, 0x11, 0x06, 0x27, 0x00, 0x12, 0x07, 0x26, 0x03, 0x14, 0x00, 0x0C, 0x13, 0x02, 0x29,
	0x14, 0x0A, 0x2B, 0x40, 0x00, 0x46, 0x08, 0x1A, 0x00, 0x22, 0x33, 0x0B, 0x22, 0x29, 0x4C, 0xFF,
	0x03, 0xFF, 0x06, 0x02, 0xFF, 0x00, 0x26, 0x01, 0x09, 0x19, 0x00, 0x1E, 0x19, 0x35, 0x27, 0x02,
	0x00, 0x1D, 0x24, 0x19, 0x15, 0x06, 0x40, 0x00, 0x46, 0x08, 0x1A, 0x00, 0x02, 0x02, 0x26, 0x03,
	0x19, 0x15, 0x02, 0x00, 0x06, 0x15, 0x0C, 0x20, 0x16, 0x00, 0x14, 0x27, 0x04, 0x27, 0x2A, 0x00,
	0x0A, 0x20, 0x01, 0x36, 0x13, 0x08, 0x29, 0x00, 0x15, 0x20, 0x3D, 0x2D, 0x00, 0x1B, 0x2F, 0x0C,
	0x16, 0x00, 0x0A, 0x27, 0x04, 0x29, 0x19, 0x3D, 0x2F, 0x10, 0xFF, 0x01, 0x00, 0x2E, 0x2E, 0x2E,
	0xFF, 0x03, 0xFF, 0x00, 0xFF, 0x01, 0x01, 0x46, 0x08, 0x19, 0x00, 0x15, 0x1F, 0x04, 0x1A, 0x00,
	0x74, 0x9F, 0x5B, 0x5C, 0x4C, 0x00, 0x72, 0x5F, 0x92, 0x50, 0x19, 0x00, 0x0C, 0x31, 0x14, 0x00,
	0x96, 0x5D, 0x54, 0x40, 0x00, 0x53, 0x9F, 0x88, 0x9D, 0x7D, 0x63, 0x14, 0x0C, 0x13, 0x00, 0x1A,
	0x10, 0x27, 0x02, 0x13, 0x02, 0x29, 0x4C, 0xFF, 0x00, 0x46, 0x08, 0x1A, 0x00, 0x01, 0x10, 0x1F,
	0x2D, 0x1C, 0x28, 0x00, 0x01, 0x10, 0x1F, 0x06, 0x27, 0x00, 0x25, 0x22, 0x2D, 0x00, 0x05, 0x02,
	0x3D, 0x0F, 0x03, 0x14, 0x0C, 0x10, 0x4C, 0xFF, 0x03, 0xFF, 0x00, 0x6E, 0x87, 0x9F, 0x93, 0x79,
	0x51, 0x7D, 0x14, 0x02, 0x03, 0x00, 0x07, 0x32, 0x3D, 0x02, 0x15, 0x00, 0x71, 0x51, 0x7D, 0x59,
	0x7D, 0x97, 0x80, 0x9F, 0x5F, 0x9F, 0x33, 0x00, 0x0E, 0x06, 0x02, 0x2D, 0x00, 0x0C, 0x1A, 0x02,
	0x0D, 0x29, 0x00, 0x36, 0x2E, 0x3D, 0x02, 0x16, 0x00, 0x05, 0x02, 0x13, 0x4D, 0x25, 0x22, 0x15,
	0x41, 0x14, 0x02, 0x03, 0x00, 0x01, 0x02, 0x1F, 0x02, 0x15, 0x00, 0x23, 0x19, 0x16, 0x00, 0x5D,
	0x7D, 0x60, 0x71, 0x7D, 0x5F, 0x78, 0x16, 0x00, 0x15, 0x29, 0x15, 0x2E, 0x13, 0xFF, 0x03, 0xFF,
	0x00, 0x64, 0x7D, 0x5D, 0x7D, 0x5C, 0x02, 0x33, 0x02, 0x19, 0x00, 0x15, 0x16, 0x23, 0x19, 0x40,
	0x23, 0x15, 0x02, 0x4C, 0xFF, 0x03, 0xFF, 0x00, 0x26, 0x19, 0x15, 0x06, 0x19, 0x00, 0x40, 0x07,
	0x37, 0x14, 0x1A, 0x00, 0x0D, 0x45, 0x13, 0x00, 0x8F, 0x88, 0x5F, 0x78, 0x15, 0x00, 0x8F, 0x9F,
	0x5F, 0x16, 0x00, 0x05, 0x07, 0x06, 0x04, 0x27, 0x2A, 0x29, 0x23, 0x19, 0x15, 0x19, 0x3D, 0x06,
	0x27, 0xFF, 0x01, 0x00, 0x2E, 0x2E, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0xFF, 0x01, 0x01, 0x46, 0x08,
	0x1A, 0x00, 0x1F, 0x41, 0x2D, 0x01, 0x09, 0x00, 0x01, 0x0B, 0x19, 0x08, 0x03, 0x07, 0x2D, 0x00,
	0x0D, 0x02, 0x0A, 0x2E, 0x3D, 0x4C, 0xFF, 0x00, 0x0D, 0x33, 0x0D, 0x33, 0x0C, 0x02, 0x00, 0x08,
	0x03, 0x07, 0x33, 0x00, 0x06, 0x27, 0x3D, 0x19, 0x15, 0x06, 0x2D, 0x00, 0x14, 0x05, 0x28, 0x17,
	0x09, 0x4D, 0x25, 0x22, 0x16, 0x00, 0x14, 0x27, 0x2C, 0x2A, 0x13, 0x02, 0x10, 0x00, 0x39, 0x44,
	0x2E, 0x33, 0x00, 0x01, 0x27, 0x02, 0x15, 0x33, 0x0B, 0x2A, 0x13, 0x02, 0x08, 0x26, 0x03, 0x15,
	0x00, 0x07, 0x33, 0x0C, 0x10, 0xFF, 0x01, 0x00, 0x2E, 0x2E, 0x2E, 0xFF, 0x02,

/* intro_french:
[if(past_intro, intro2)][load(SCENE_BLACK)]Je fais souvent le même cauchemar...[pause]
[load(SCENE_INTRO_1)]Une [purple]jeune fille[white] combat un monstrueux démon.[wait]
Je les regarde se battre, mais je ne peux ni bouger, ni parler. Tout ce que je peux faire, c'est assister impuissant aux assauts du monstre.[wait][load(SCENE_BLACK)]
À chaque fois, je me réveille au moment où elle va succomber.[wait][load(SCENE_INTRO_2)]
Je me réveille dans mon lit, qu'éclairent les premiers rayons de l'aube. Une immense tristesse m'étreint. Je tente désespérément de refouler les larmes qui me montent aux yeux...[wait]
Nous vivons dans l'époque moderne, sous les auspices du super-ordinateur Mother Brain. Il n'est plus temps de s'épancher dans la futilité des rêves.[wait]
L'heure est à la logique d'un monde régi par les données numériques.[wait]
J'ouvre ma fenêtre et prend une bouffée d'air frais.[wait]
La brise matinale se répand dans mon corps, dispersant les derniers souvenirs de mon cauchemar.[set(past_intro)][close]*/
	0xFF, 0x0C, 0x00, 0x00, 0xFF, 0xFF, 0x03, 0x8B, 0xFF, 0x06, 0x00, 0x4A, 0x65, 0x00, 0x66, 0x61,
	0x69, 0x73, 0x00, 0x73, 0x6F, 0x75, 0x76, 0x65, 0x6E, 0x74, 0x00, 0x6C, 0x65, 0x00, 0x6D, 0x85,
	0x6D, 0x65, 0x00, 0x63, 0x61, 0x75, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x72, 0x2E, 0x2E, 0x2E, 0xFF,
	0x04, 0xFF, 0x00, 0xFF, 0x06, 0x01, 0x55, 0x6E, 0x65, 0x00, 0xFF, 0x09, 0x6A, 0x65, 0x75, 0x6E,
	0x65, 0x00, 0x66, 0x69, 0x6C, 0x6C, 0x65, 0xFF, 0x07, 0x00, 0x63, 0x6F, 0x6D, 0x62, 0x61, 0x74,
	0x00, 0x75, 0x6E, 0x00, 0x6D, 0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75, 0x65, 0x75, 0x78, 0x00, 0x64,
	0x83, 0x6D, 0x6F, 0x6E, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0x4A, 0x65, 0x00, 0x6C, 0x65, 0x73, 0x00,
	0x72, 0x65, 0x67, 0x61, 0x72, 0x64, 0x65, 0x00, 0x73, 0x65, 0x00, 0x62, 0x61, 0x74, 0x74, 0x72,
	0x65, 0x2C, 0x00, 0x6D, 0x61, 0x69, 0x73, 0x00, 0x6A, 0x65, 0x00, 0x6E, 0x65, 0x00, 0x70, 0x65,
	0x75, 0x78, 0x00, 0x6E, 0x69, 0x00, 0x62, 0x6F, 0x75, 0x67, 0x65, 0x72, 0x2C, 0x00, 0x6E, 0x69,
	0x00, 0x70, 0x61, 0x72, 0x6C, 0x65, 0x72, 0x2E, 0x00, 0x54, 0x6F, 0x75, 0x74, 0x00, 0x63, 0x65,
	0x00, 0x71, 0x75, 0x65, 0x00, 0x6A, 0x65, 0x00, 0x70, 0x65, 0x75, 0x78, 0x00, 0x66, 0x61, 0x69,
	0x72, 0x65, 0x2C, 0x00, 0x63, 0x27, 0x65, 0x73, 0x74, 0x00, 0x61, 0x73, 0x73, 0x69, 0x73, 0x74,
	0x65, 0x72, 0x00, 0x69, 0x6D, 0x70, 0x75, 0x69, 0x73, 0x73, 0x61, 0x6E, 0x74, 0x00, 0x61, 0x75,
	0x78, 0x00, 0x61, 0x73, 0x73, 0x61, 0x75, 0x74, 0x73, 0x00, 0x64, 0x75, 0x00, 0x6D, 0x6F, 0x6E,
	0x73, 0x74, 0x72, 0x65, 0x2E, 0xFF, 0x03, 0xFF, 0x06, 0x00, 0xFF, 0x00, 0x90, 0x00, 0x63, 0x68,
	0x61, 0x71, 0x75, 0x65, 0x00, 0x66, 0x6F, 0x69, 0x73, 0x2C, 0x00, 0x6A, 0x65, 0x00, 0x6D, 0x65,
	0x00, 0x72, 0x83, 0x76, 0x65, 0x69, 0x6C, 0x6C, 0x65, 0x00, 0x61, 0x75, 0x00, 0x6D, 0x6F, 0x6D,
	0x65, 0x6E, 0x74, 0x00, 0x6F, 0x8B, 0x00, 0x65, 0x6C, 0x6C, 0x65, 0x00, 0x76, 0x61, 0x00, 0x73,
	0x75, 0x63, 0x63, 0x6F, 0x6D, 0x62, 0x65, 0x72, 0x2E, 0xFF, 0x03, 0xFF, 0x06, 0x02, 0xFF, 0x00,
	0x4A, 0x65, 0x00, 0x6D, 0x65, 0x00, 0x72, 0x83, 0x76, 0x65, 0x69, 0x6C, 0x6C, 0x65, 0x00, 0x64,
	0x61, 0x6E, 0x73, 0x00, 0x6D, 0x6F, 0x6E, 0x00, 0x6C, 0x69, 0x74, 0x2C, 0x00, 0x71, 0x75, 0x27,
	0x83, 0x63, 0x6C, 0x61, 0x69, 0x72, 0x65, 0x6E, 0x74, 0x00, 0x6C, 0x65, 0x73, 0x00, 0x70, 0x72,
	0x65, 0x6D, 0x69, 0x65, 0x72, 0x73, 0x00, 0x72, 0x61, 0x79, 0x6F, 0x6E, 0x73, 0x00, 0x64, 0x65,
	0x00, 0x6C, 0x27, 0x61, 0x75, 0x62, 0x65, 0x2E, 0x00, 0x55, 0x6E, 0x65, 0x00, 0x69, 0x6D, 0x6D,
	0x65, 0x6E, 0x73, 0x65, 0x00, 0x74, 0x72, 0x69, 0x73, 0x74, 0x65, 0x73, 0x73, 0x65, 0x00, 0x6D,
	0x27, 0x83, 0x74, 0x72, 0x65, 0x69, 0x6E, 0x74, 0x2E, 0x00, 0x4A, 0x65, 0x00, 0x74, 0x65, 0x6E,
	0x74, 0x65, 0x00, 0x64, 0x83, 0x73, 0x65, 0x73, 0x70, 0x83, 0x72, 0x83, 0x6D, 0x65, 0x6E, 0x74,
	0x00, 0x64, 0x65, 0x00, 0x72, 0x65, 0x66, 0x6F, 0x75, 0x6C, 0x65, 0x72, 0x00, 0x6C, 0x65, 0x73,
	0x00, 0x6C, 0x61, 0x72, 0x6D, 0x65, 0x73, 0x00, 0x71, 0x75, 0x69, 0x00, 0x6D, 0x65, 0x00, 0x6D,
	0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x00, 0x61, 0x75, 0x78, 0x00, 0x79, 0x65, 0x75, 0x78, 0x2E,
	0x2E, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0x4E, 0x6F, 0x75, 0x73, 0x00, 0x76, 0x69, 0x76, 0x6F, 0x6E,
	0x73, 0x00, 0x64, 0x61, 0x6E, 0x73, 0x00, 0x6C, 0x27, 0x83, 0x70, 0x6F, 0x71, 0x75, 0x65, 0x00,
	0x6D, 0x6F, 0x64, 0x65, 0x72, 0x6E, 0x65, 0x2C, 0x00, 0x73, 0x6F, 0x75, 0x73, 0x00, 0x6C, 0x65,
	0x73, 0x00, 0x61, 0x75, 0x73, 0x70, 0x69, 0x63, 0x65, 0x73, 0x00, 0x64, 0x75, 0x00, 0x73, 0x75,
	0x70, 0x65, 0x72, 0x2D, 0x6F, 0x72, 0x64, 0x69, 0x6E, 0x61, 0x74, 0x65, 0x75, 0x72, 0x00, 0x4D,
	0x6F, 0x74, 0x68, 0x65, 0x72, 0x00, 0x42, 0x72, 0x61, 0x69, 0x6E, 0x2E, 0x00, 0x49, 0x6C, 0x00,
	0x6E, 0x27, 0x65, 0x73, 0x74, 0x00, 0x70, 0x6C, 0x75, 0x73, 0x00, 0x74, 0x65, 0x6D, 0x70, 0x73,
	0x00, 0x64, 0x65, 0x00, 0x73, 0x27, 0x83, 0x70, 0x61, 0x6E, 0x63, 0x68, 0x65, 0x72, 0x00, 0x64,
	0x61, 0x6E, 0x73, 0x00, 0x6C, 0x61, 0x00, 0x66, 0x75, 0x74, 0x69, 0x6C, 0x69, 0x74, 0x83, 0x00,
	0x64, 0x65, 0x73, 0x00, 0x72, 0x85, 0x76, 0x65, 0x73, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0x4C, 0x27,
	0x68, 0x65, 0x75, 0x72, 0x65, 0x00, 0x65, 0x73, 0x74, 0x00, 0x80, 0x00, 0x6C, 0x61, 0x00, 0x6C,
	0x6F, 0x67, 0x69, 0x71, 0x75, 0x65, 0x00, 0x64, 0x27, 0x75, 0x6E, 0x00, 0x6D, 0x6F, 0x6E, 0x64,
	0x65, 0x00, 0x72, 0x83, 0x67, 0x69, 0x00, 0x70, 0x61, 0x72, 0x00, 0x6C, 0x65, 0x73, 0x00, 0x64,
	0x6F, 0x6E, 0x6E, 0x83, 0x65, 0x73, 0x00, 0x6E, 0x75, 0x6D, 0x83, 0x72, 0x69, 0x71, 0x75, 0x65,
	0x73, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0x4A, 0x27, 0x6F, 0x75, 0x76, 0x72, 0x65, 0x00, 0x6D, 0x61,
	0x00, 0x66, 0x65, 0x6E, 0x85, 0x74, 0x72, 0x65, 0x00, 0x65, 0x74, 0x00, 0x70, 0x72, 0x65, 0x6E,
	0x64, 0x00, 0x75, 0x6E, 0x65, 0x00, 0x62, 0x6F, 0x75, 0x66, 0x66, 0x83, 0x65, 0x00, 0x64, 0x27,
	0x61, 0x69, 0x72, 0x00, 0x66, 0x72, 0x61, 0x69, 0x73, 0x2E, 0xFF, 0x03, 0xFF, 0x00, 0x4C, 0x61,
	0x00, 0x62, 0x72, 0x69, 0x73, 0x65, 0x00, 0x6D, 0x61, 0x74, 0x69, 0x6E, 0x61, 0x6C, 0x65, 0x00,
	0x73, 0x65, 0x00, 0x72, 0x83, 0x70, 0x61, 0x6E, 0x64, 0x00, 0x64, 0x61, 0x6E, 0x73, 0x00, 0x6D,
	0x6F, 0x6E, 0x00, 0x63, 0x6F, 0x72, 0x70, 0x73, 0x2C, 0x00, 0x64, 0x69, 0x73, 0x70, 0x65, 0x72,
	0x73, 0x61, 0x6E, 0x74, 0x00, 0x6C, 0x65, 0x73, 0x00, 0x64, 0x65, 0x72, 0x6E, 0x69, 0x65, 0x72,
	0x73, 0x00, 0x73, 0x6F, 0x75, 0x76, 0x65, 0x6E, 0x69, 0x72, 0x73, 0x00, 0x64, 0x65, 0x00, 0x6D,
	0x6F, 0x6E, 0x00, 0x63, 0x61, 0x75, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x72, 0x2E, 0xFF, 0x0E, 0x00,
	0x00, 0xFF, 0x02,

/* intro2_english:
You have passed intro[close]*/
	0x59, 0x6F, 0x75, 0x00, 0x68, 0x61, 0x76, 0x65, 0x00, 0x70, 0x61, 0x73, 0x73, 0x65, 0x64, 0x00,
	0x69, 0x6E, 0x74, 0x72, 0x6F, 0xFF, 0x02,

/* intro2_japanese:
ひとりの[close]*/
	0xFF, 0x01, 0x01, 0x1B, 0x14, 0x28, 0x19, 0xFF, 0x02,

/* intro2_french:
Vous avez passé l'intro[clr(past_intro)][close]*/
	0x56, 0x6F, 0x75, 0x73, 0x00, 0x61, 0x76, 0x65, 0x7A, 0x00, 0x70, 0x61, 0x73, 0x73, 0x83, 0x00,
	0x6C, 0x27, 0x69, 0x6E, 0x74, 0x72, 0x6F, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0x02
};