// ====================================================
// DESK BUDDY  –  Standalone Offline Companion  v2.0
// Edison Science Corner  –  ESCLabs  –  esclabs.in
// ====================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>
#include <math.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

// ────────────────────────────────────────────────────
// HARDWARE
// ────────────────────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define SDA_PIN         6
#define SCL_PIN         7
#define TOUCH_PIN       4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Preferences prefs;

// ────────────────────────────────────────────────────
// PROGMEM BITMAPS  (unchanged from original)
// ────────────────────────────────────────────────────
const unsigned char bmp_heart[] PROGMEM = {
  0x00,0x00, 0x0c,0x60, 0x1e,0xf0, 0x3f,0xf8,
  0x7f,0xfc, 0x7f,0xfc, 0x7f,0xfc, 0x3f,0xf8,
  0x1f,0xf0, 0x0f,0xe0, 0x07,0xc0, 0x03,0x80,
  0x01,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00
};
const unsigned char bmp_zzz[] PROGMEM = {
  0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x3c,
  0x00,0x0c, 0x00,0x18, 0x00,0x30, 0x00,0x7e,
  0x00,0x00, 0x3c,0x00, 0x0c,0x00, 0x18,0x00,
  0x30,0x00, 0x7c,0x00, 0x00,0x00, 0x00,0x00
};
const unsigned char bmp_anger[] PROGMEM = {
  0x00,0x00, 0x11,0x10, 0x2a,0x90, 0x44,0x40,
  0x80,0x20, 0x80,0x20, 0x44,0x40, 0x2a,0x90,
  0x11,0x10, 0x00,0x00, 0x00,0x00, 0x00,0x00,
  0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00
};
const unsigned char bmp_book[] PROGMEM = {
  0x7f,0xfe, 0x40,0x02, 0x5f,0xfa, 0x5f,0xfa,
  0x5f,0xfa, 0x5f,0xfa, 0x5f,0xfa, 0x5f,0xfa,
  0x5f,0xfa, 0x5f,0xfa, 0x40,0x02, 0x7f,0xfe,
  0x3f,0xfc, 0x0f,0xf0, 0x03,0xc0, 0x01,0x80
};
const unsigned char bmp_coffee[] PROGMEM = {
  0x00,0x00, 0x0a,0x80, 0x05,0x00, 0x0a,0x80,
  0x3f,0xf0, 0x20,0x18, 0x20,0x0c, 0x20,0x0c,
  0x20,0x18, 0x3f,0xf0, 0x1f,0xe0, 0x0f,0xc0,
  0x07,0x80, 0x00,0x00, 0x7f,0xfe, 0x00,0x00
};
const unsigned char bmp_star[] PROGMEM = {
  0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x28, 0x44
};
const unsigned char bmp_dino_run1[] PROGMEM = {
  0x00,0x7c, 0x00,0xfe, 0x00,0xee, 0x00,0xfe,
  0x07,0xf0, 0x0f,0xf8, 0x1f,0xf8, 0x1f,0xf0,
  0x1f,0xe0, 0x0f,0xc0, 0x07,0x80, 0x03,0x80,
  0x02,0x40, 0x02,0x40, 0x03,0x00, 0x02,0x00
};
const unsigned char bmp_dino_run2[] PROGMEM = {
  0x00,0x7c, 0x00,0xfe, 0x00,0xee, 0x00,0xfe,
  0x07,0xf0, 0x0f,0xf8, 0x1f,0xf8, 0x1f,0xf0,
  0x1f,0xe0, 0x0f,0xc0, 0x07,0x80, 0x03,0x80,
  0x01,0x20, 0x01,0x20, 0x00,0xc0, 0x00,0x40
};
const unsigned char bmp_dino_jump[] PROGMEM = {
  0x00,0x7c, 0x00,0xfe, 0x00,0xee, 0x00,0xfe,
  0x07,0xf0, 0x0f,0xf8, 0x1f,0xf8, 0x1f,0xf0,
  0x1f,0xe0, 0x0f,0xc0, 0x07,0x80, 0x03,0x80,
  0x02,0x40, 0x02,0x40, 0x03,0xc0, 0x00,0x00
};
const unsigned char bmp_cactus1[] PROGMEM = {
  0x18, 0x18, 0x58, 0xd8, 0xd8, 0xff,
  0xd8, 0x18, 0x18, 0x18, 0x18, 0x18
};
const unsigned char bmp_cactus2[] PROGMEM = {
  0x0c,0x30, 0x0c,0x30, 0x2c,0xb0, 0x6c,0xb0,
  0x6c,0xf0, 0x7f,0xf0, 0x6c,0x30, 0x0c,0x30,
  0x0c,0x30, 0x0c,0x30, 0x0c,0x30, 0x0c,0x30
};
const unsigned char bmp_rock[] PROGMEM = {
  0x3c, 0x7e, 0xff, 0xff, 0xdb, 0xff
};

// ────────────────────────────────────────────────────
// APP STATE MACHINE
// ────────────────────────────────────────────────────
#define APP_BOOT      0   // Boot animation running
#define APP_HELLO     1   // "Hello !!!" — waiting for single tap
#define APP_MENU      2   // Main menu
#define APP_CLOCK     3   // Digital clock
#define APP_POMODORO  4   // Pomodoro timer
#define APP_MOODS     5   // Mood face selector
#define APP_PET       6   // Virtual pet
#define APP_DINO      7   // Dino game
#define APP_SETTINGS  8   // Settings
int appState = APP_BOOT;

// ────────────────────────────────────────────────────
// MAIN MENU
// ────────────────────────────────────────────────────
#define MENU_COUNT 6
const char* MENU_ITEMS[MENU_COUNT] = {
  "Clock", "Pomodoro", "Moods", "Pet", "Dino Game", "Settings"
};
int menuCursor = 0;

// ────────────────────────────────────────────────────
// SETTINGS MENU
// ────────────────────────────────────────────────────
#define SETTINGS_COUNT 4
const char* SETTINGS_ITEMS[SETTINGS_COUNT] = {
  "Brightness", "About", "Reset HiScore", "Reset All"
};
int settingsCursor = 0;
bool inAbout = false;

// ────────────────────────────────────────────────────
// TOUCH
// ────────────────────────────────────────────────────
int   tapCounter       = 0;
unsigned long lastTapTime    = 0;
bool  lastPinState     = false;
unsigned long pressStart     = 0;
bool  longHeld         = false;
const unsigned long DOUBLE_TAP_MS = 280UL;
const unsigned long LONG_PRESS_MS = 900UL;

// ────────────────────────────────────────────────────
// FRAME TIMING  (~40 FPS)
// ────────────────────────────────────────────────────
unsigned long lastFrameTime = 0;
const unsigned long FRAME_DELAY = 25UL;

// ────────────────────────────────────────────────────
// BRIGHTNESS
// ────────────────────────────────────────────────────
bool highBrightness = true;

// ────────────────────────────────────────────────────
// MOOD SYSTEM
// ────────────────────────────────────────────────────
#define MOOD_NORMAL     0
#define MOOD_HAPPY      1
#define MOOD_SURPRISED  2
#define MOOD_SLEEPY     3
#define MOOD_ANGRY      4
#define MOOD_SAD        5
#define MOOD_EXCITED    6
#define MOOD_LOVE       7
#define MOOD_SUSPICIOUS 8

int idleMood   = MOOD_HAPPY;   // Saved idle face
int moodSelIdx = 0;             // Current selection in mood picker

#define MOOD_LIST_COUNT 6
const int   MOOD_LIST[MOOD_LIST_COUNT]  = {
  MOOD_HAPPY, MOOD_SAD, MOOD_ANGRY, MOOD_LOVE, MOOD_SURPRISED, MOOD_SUSPICIOUS
};
const char* MOOD_NAMES[MOOD_LIST_COUNT] = {
  "Happy", "Sad", "Angry", "Love", "Cry", "Weird"
};

// ────────────────────────────────────────────────────
// POMODORO
// ────────────────────────────────────────────────────
#define POMO_SETUP   0
#define POMO_RUNNING 1
#define POMO_DONE    2
int   pomoState    = POMO_SETUP;
int   pomoDuration = 5;          // minutes — cycles 5→10→…→60→5
// pomoCursor: 0=editing duration, 1=Start button, 2=Back button
int   pomoCursor   = 0;
unsigned long pomoStart  = 0;
unsigned long pomoMs     = 0;
unsigned long pomoDoneAt = 0;
int   completedSessions  = 0;

// ────────────────────────────────────────────────────
// VIRTUAL PET
// ────────────────────────────────────────────────────
float petHappiness   = 50.0f;
unsigned long lastPetDecay = 0;
bool  isPetJumping   = false;
unsigned long petJumpStart = 0;

// ────────────────────────────────────────────────────
// DINO GAME  (variables unchanged from original)
// ────────────────────────────────────────────────────
#define DINO_START    0
#define DINO_PLAY     1
#define DINO_OVER     2
#define DINO_HI       3
int   dinoState   = DINO_START;
float dinoY       = 34.0f;
float dinoVelY    = 0.0f;
const float GRAV  = 0.55f;
const float JUMPF = -7.5f;
bool  dinoJumping = false;

struct Obstacle { float x; int y, w, h; bool active; int type; };
Obstacle obs[2];

float cloudX = 128.0f, cloudY = 12.0f, gndOff = 0.0f;
unsigned long dinoScore = 0;
unsigned int  dinoHi    = 0;
float gSpeed = 3.0f;
unsigned long lastDinoIn = 0;

// ────────────────────────────────────────────────────
// EYE PHYSICS ENGINE  (unchanged from original)
// ────────────────────────────────────────────────────
struct Eye {
  float x,  y,  w,  h;
  float tX, tY, tW, tH;
  float pX,  pY,  tPX, tPY;
  float vX,  vY,  vW,  vH;
  float pvX, pvY;
  float k  = 0.12f, d  = 0.60f;
  float pk = 0.08f, pd = 0.50f;
  bool  blink;
  unsigned long blinkAt, nextBlink;

  void init(float _x, float _y, float _w, float _h) {
    x=tX=_x;  y=tY=_y;  w=tW=_w;  h=tH=_h;
    pX=tPX=pY=tPY=0;
    vX=vY=vW=vH=pvX=pvY=0;
    blink=false;  blinkAt=0;
    nextBlink = millis() + random(2000, 5000);
  }

  void update() {
    vX=(vX+(tX-x)*k)*d;  vY=(vY+(tY-y)*k)*d;
    vW=(vW+(tW-w)*k)*d;  vH=(vH+(tH-h)*k)*d;
    x+=vX;  y+=vY;  w+=vW;  h+=vH;
    pvX=(pvX+(tPX-pX)*pk)*pd;
    pvY=(pvY+(tPY-pY)*pk)*pd;
    pX+=pvX;  pY+=pvY;
  }
};

Eye L, R;
unsigned long lastSaccade = 0;
unsigned long saccInt     = 3000;
float breathVal = 0;

// ────────────────────────────────────────────────────
// BOOT ANIMATION GLOBALS
// ────────────────────────────────────────────────────
int bootStep = 0;
unsigned long bootStepT = 0;

// ════════════════════════════════════════════════════
// DRAWING PRIMITIVES  (unchanged from original)
// ════════════════════════════════════════════════════

void drawEyelidMask(float x, float y, float w, float h, int mood, bool isLeft) {
  int ix=(int)x, iy=(int)y, iw=(int)w, ih=(int)h;
  if (mood == MOOD_ANGRY) {
    if (isLeft) for(int i=0;i<16;i++) display.drawLine(ix,iy+i,ix+iw,iy-6+i,SSD1306_BLACK);
    else        for(int i=0;i<16;i++) display.drawLine(ix,iy-6+i,ix+iw,iy+i,SSD1306_BLACK);
  } else if (mood == MOOD_SAD) {
    if (isLeft) for(int i=0;i<16;i++) display.drawLine(ix,iy-6+i,ix+iw,iy+i,SSD1306_BLACK);
    else        for(int i=0;i<16;i++) display.drawLine(ix,iy+i,ix+iw,iy-6+i,SSD1306_BLACK);
  } else if (mood == MOOD_HAPPY || mood == MOOD_LOVE || mood == MOOD_EXCITED) {
    display.fillRect(ix, iy+ih-10, iw, 12, SSD1306_BLACK);
    display.fillCircle(ix+iw/2, iy+ih+6, (int)(iw/1.3f), SSD1306_BLACK);
  } else if (mood == MOOD_SLEEPY) {
    display.fillRect(ix, iy, iw, ih/2+2, SSD1306_BLACK);
  } else if (mood == MOOD_SUSPICIOUS) {
    if (isLeft) display.fillRect(ix, iy, iw, ih/2-2, SSD1306_BLACK);
    else        display.fillRect(ix, iy+ih-8, iw, 8, SSD1306_BLACK);
  }
}

void drawEye(Eye& e, bool isLeft, int mood) {
  int ix=(int)e.x, iy=(int)e.y, iw=(int)e.w, ih=(int)e.h;
  if (iw < 2 || ih < 2) return;
  int r = (iw < 20) ? 3 : 6;
  display.fillRoundRect(ix, iy, iw, ih, r, SSD1306_WHITE);
  int cx=ix+iw/2, cy=iy+ih/2;
  int pw=(int)(iw/2.2f), ph=(int)(ih/2.2f);
  if (pw < 1) pw=1; if (ph < 1) ph=1;
  int px=cx+(int)e.pX-(pw/2), py=cy+(int)e.pY-(ph/2);
  if (px < ix)       px=ix;
  if (px+pw > ix+iw) px=ix+iw-pw;
  if (py < iy)       py=iy;
  if (py+ph > iy+ih) py=iy+ih-ph;
  display.fillRoundRect(px, py, pw, ph, r/2, SSD1306_BLACK);
  if (iw > 15 && ih > 15) display.fillCircle(px+pw-4, py+4, 2, SSD1306_WHITE);
  drawEyelidMask(e.x, e.y, e.w, e.h, mood, isLeft);
}

void drawMouth(int cx, int cy, int mood) {
  switch (mood) {
    case MOOD_HAPPY: case MOOD_LOVE: case MOOD_EXCITED:
      display.fillCircle(cx, cy, 6, SSD1306_WHITE);
      display.fillRect(cx-7, cy-7, 14, 8, SSD1306_BLACK);
      break;
    case MOOD_SAD: case MOOD_ANGRY:
      display.fillCircle(cx, cy, 6, SSD1306_WHITE);
      display.fillRect(cx-7, cy-1, 14, 8, SSD1306_BLACK);
      break;
    case MOOD_SURPRISED:
      display.drawCircle(cx, cy, 4, SSD1306_WHITE);
      display.drawCircle(cx, cy, 5, SSD1306_WHITE);
      break;
    case MOOD_SLEEPY:
      display.drawCircle(cx, cy, 2, SSD1306_WHITE);
      break;
    case MOOD_SUSPICIOUS:
      for(int t=0;t<2;t++) display.drawLine(cx-10,cy+2+t,cx+10,cy-2+t,SSD1306_WHITE);
      break;
    default:
      for(int t=0;t<2;t++) display.drawLine(cx-8,cy+t,cx+8,cy+t,SSD1306_WHITE);
      break;
  }
}

// ════════════════════════════════════════════════════
// EYE PHYSICS UPDATE  (adapted from updatePhysicsAndMood)
//   eyY  = base Y position of eyes
//   eyW/H = base width/height
//   mood  = active face mood
//   Lx/Rx = left/right eye base X (default = original positions)
// ════════════════════════════════════════════════════
void updateEyes(float eyY, float eyW, float eyH, int mood,
                float Lx=18.0f, float Rx=74.0f) {
  unsigned long now = millis();
  breathVal = sinf(now / 800.0f) * 1.5f;

  // ── Auto-blink ──────────────────────────────────
  if (now > L.nextBlink) {
    L.blink = R.blink = true;
    L.blinkAt = now;
    L.nextBlink = now + random(2000, 6000);
  }
  if (L.blink) {
    L.tH = R.tH = 2;
    if (now - L.blinkAt > 120) L.blink = R.blink = false;
  }

  // ── Saccade (gaze micro-movement) ───────────────
  if (!L.blink && now - lastSaccade > saccInt) {
    lastSaccade = now;
    saccInt = random(500, 3000);
    int dir = random(0, 10);
    float lx=0, ly=0;
    if      (dir < 4) { lx= 0; ly= 0; }
    else if (dir==4)  { lx=-6; ly=-4; }
    else if (dir==5)  { lx= 6; ly=-4; }
    else if (dir==6)  { lx=-6; ly= 4; }
    else if (dir==7)  { lx= 6; ly= 4; }
    else if (dir==8)  { lx= 8; ly= 0; }
    else              { lx=-8; ly= 0; }
    L.tPX=lx; L.tPY=ly; R.tPX=lx; R.tPY=ly;
    L.tX = Lx + lx*0.3f;  L.tY = eyY + ly*0.3f;
    R.tX = Rx + lx*0.3f;  R.tY = eyY + ly*0.3f;
  }

  // ── Mood shape overrides ─────────────────────────
  if (!L.blink) {
    float eh = eyH + breathVal;
    switch (mood) {
      case MOOD_HAPPY: case MOOD_LOVE:
        L.tW=R.tW=40; L.tH=R.tH=32; break;
      case MOOD_SURPRISED:
        L.tW=R.tW=eyW-6; L.tH=R.tH=eh+8; break;
      case MOOD_SLEEPY:
        L.tW=R.tW=eyW+2; L.tH=R.tH=eh-6; break;
      case MOOD_ANGRY:
        L.tW=R.tW=eyW-2; L.tH=R.tH=eh-4; break;
      case MOOD_SAD:
        L.tW=R.tW=eyW-2; L.tH=R.tH=eh+4; break;
      case MOOD_SUSPICIOUS:
        L.tW=R.tW=eyW; L.tH=eh-12; R.tH=eh+6; break;
      default:
        L.tW=R.tW=eyW; L.tH=R.tH=eh; break;
    }
  }
  L.update(); R.update();
}

// ════════════════════════════════════════════════════
// BOOT ANIMATION  (7 steps, ~5 s total)
// ════════════════════════════════════════════════════
void nextStep() { bootStep++; bootStepT = millis(); }

void runBoot() {
  unsigned long el = millis() - bootStepT;

  switch (bootStep) {

    case 0:  // ── Black screen 300 ms ─────────────
      display.clearDisplay();
      display.display();
      if (el > 300) nextStep();
      break;

    case 1:  // ── Eyes grow in ────────────────────
      L.tX=18; L.tY=14; L.tW=36; L.tH=24;
      R.tX=74; R.tY=14; R.tW=36; R.tH=24;
      L.tPX=L.tPY=R.tPX=R.tPY=0;
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_NORMAL);
      drawEye(R, false, MOOD_NORMAL);
      display.display();
      if (el > 700) nextStep();
      break;

    case 2:  // ── Look left ───────────────────────
      L.tPX=R.tPX=-8; L.tPY=R.tPY=0;
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_NORMAL);
      drawEye(R, false, MOOD_NORMAL);
      display.display();
      if (el > 500) nextStep();
      break;

    case 3:  // ── Look right ──────────────────────
      L.tPX=R.tPX=8; L.tPY=R.tPY=0;
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_NORMAL);
      drawEye(R, false, MOOD_NORMAL);
      display.display();
      if (el > 500) nextStep();
      break;

    case 4:  // ── Blink 1 ─────────────────────────
      L.tPX=R.tPX=0; L.tPY=R.tPY=0;
      L.tH = R.tH = (el < 150) ? 2.0f : 24.0f;
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_NORMAL);
      drawEye(R, false, MOOD_NORMAL);
      display.display();
      if (el > 380) nextStep();
      break;

    case 5:  // ── Blink 2 ─────────────────────────
      L.tH = R.tH = (el < 150) ? 2.0f : 24.0f;
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_NORMAL);
      drawEye(R, false, MOOD_NORMAL);
      display.display();
      if (el > 380) nextStep();
      break;

    case 6:  // ── Centre, switch to HAPPY ──────────
      L.tPX=L.tPY=R.tPX=R.tPY=0;
      L.tH=R.tH=24;
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_NORMAL);
      drawEye(R, false, MOOD_NORMAL);
      display.display();
      if (el > 350) nextStep();
      break;

    case 7: { // ── Hello !!! (2 s) ─────────────────
      L.update(); R.update();
      display.clearDisplay();
      drawEye(L, true,  MOOD_HAPPY);
      drawEye(R, false, MOOD_HAPPY);
      drawMouth(64, 40, MOOD_HAPPY);
      display.setFont(&FreeSansBold9pt7b);
      display.setTextColor(SSD1306_WHITE);
      int16_t bx,by; uint16_t bw,bh;
      display.getTextBounds("Hello !!!", 0, 0, &bx, &by, &bw, &bh);
      display.setCursor((SCREEN_WIDTH-(int)bw)/2, 62);
      display.print("Hello !!!");
      display.setFont(NULL);
      display.display();
      if (el > 2000) appState = APP_HELLO;
      break;
    }
  }
}

// ════════════════════════════════════════════════════
// HELLO SCREEN  (eyes alive while waiting for tap)
// ════════════════════════════════════════════════════
void drawHello() {
  updateEyes(10, 36, 24, MOOD_HAPPY);
  display.clearDisplay();
  drawEye(L, true,  MOOD_HAPPY);
  drawEye(R, false, MOOD_HAPPY);
  drawMouth(64, 40, MOOD_HAPPY);
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(SSD1306_WHITE);
  int16_t bx,by; uint16_t bw,bh;
  display.getTextBounds("Hello !!!", 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH-(int)bw)/2, 62);
  display.print("Hello !!!");
  display.setFont(NULL);
  display.display();
}

// ════════════════════════════════════════════════════
// MAIN MENU
// ════════════════════════════════════════════════════
void drawMenu() {
  display.clearDisplay();
  display.setFont(NULL);
  display.setTextSize(1);

  // Title bar
  display.fillRect(0, 0, 128, 11, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  int16_t bx,by; uint16_t bw,bh;
  display.getTextBounds("DESK BUDDY", 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH-(int)bw)/2, 2);
  display.print("DESK BUDDY");

  // Scroll so selected item is always visible (max 4 rows shown)
  int scroll = 0;
  if (menuCursor > 3) scroll = menuCursor - 3;

  for (int i = 0; i < 4 && (i+scroll) < MENU_COUNT; i++) {
    int idx  = i + scroll;
    int yRow = 12 + i * 13;
    bool sel = (idx == menuCursor);

    if (sel) {
      display.fillRect(0, yRow, 128, 13, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(4, yRow+3);
      display.print("> ");
    } else {
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(12, yRow+3);
    }
    display.print(MENU_ITEMS[idx]);
  }
  display.setTextColor(SSD1306_WHITE);

  // Scroll indicator strip on right edge
  if (MENU_COUNT > 4) {
    int barH = max(4, 52 / MENU_COUNT);
    int barY = 12 + menuCursor * (52 - barH) / (MENU_COUNT - 1);
    display.fillRect(126, barY, 2, barH, SSD1306_WHITE);
  }

  display.display();
}

// ════════════════════════════════════════════════════
// CLOCK  (unchanged drawing logic from original)
// ════════════════════════════════════════════════════
void drawClock() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  struct tm t;

  if (!getLocalTime(&t)) {
    display.setFont(NULL);
    display.setTextSize(1);
    display.setCursor(18, 10);  display.print("Time not synced.");
    display.setCursor(10, 24);  display.print("(Offline device)");
    display.setCursor(14, 38);  display.print("--:-- --");
    display.setCursor(22, 54);  display.print("Tap: Menu");
    display.display();
    return;
  }

  String ap = (t.tm_hour >= 12) ? "PM" : "AM";
  int h = t.tm_hour % 12;
  if (h == 0) h = 12;

  display.setFont(NULL);
  display.setTextSize(1);
  display.setCursor(114, 0);
  display.print(ap);

  display.setFont(&FreeSansBold18pt7b);
  char ts[6];
  sprintf(ts, "%02d:%02d", h, t.tm_min);
  int16_t x1,y1; uint16_t w,hh;
  display.getTextBounds(ts, 0, 0, &x1, &y1, &w, &hh);
  display.setCursor((SCREEN_WIDTH-(int)w)/2, 42);
  display.print(ts);

  display.setFont(&FreeSans9pt7b);
  char ds[20];
  strftime(ds, 20, "%a, %b %d", &t);
  display.getTextBounds(ds, 0, 0, &x1, &y1, &w, &hh);
  display.setCursor((SCREEN_WIDTH-(int)w)/2, 62);
  display.print(ds);

  display.display();
}

// ════════════════════════════════════════════════════
// POMODORO
// ════════════════════════════════════════════════════
void drawPomodoro() {
  display.clearDisplay();
  display.setFont(NULL);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (pomoState == POMO_SETUP) {
    // Small eyes at top (original Pomodoro positions)
    updateEyes(4, 26, 18, MOOD_HAPPY, 26, 76);
    drawEye(L, true,  MOOD_HAPPY);
    drawEye(R, false, MOOD_HAPPY);
    drawMouth(64, 26, MOOD_HAPPY);

    // Duration box (highlighted when pomoCursor==0)
    if (pomoCursor == 0) {
      display.fillRect(30, 29, 68, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.drawRect(30, 29, 68, 12, SSD1306_WHITE);
    }
    char dur[12];
    sprintf(dur, "  %02d min", pomoDuration);
    display.setCursor(34, 32);
    display.print(dur);
    display.setTextColor(SSD1306_WHITE);

    // ── Start button ───────────────────────────────
    bool startSel = (pomoCursor == 1);
    if (startSel) {
      display.fillRect(4,  43, 56, 11, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.drawRect(4, 43, 56, 11, SSD1306_WHITE);
    }
    display.setCursor(14, 46);
    display.print("Start");
    display.setTextColor(SSD1306_WHITE);

    // ── Back button ────────────────────────────────
    bool backSel = (pomoCursor == 2);
    if (backSel) {
      display.fillRect(68, 43, 56, 11, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.drawRect(68, 43, 56, 11, SSD1306_WHITE);
    }
    display.setCursor(76, 46);
    display.print("Back");
    display.setTextColor(SSD1306_WHITE);

    // Hint
    display.setCursor(4, 57);
    display.print("Tap:+5m  2xTap:Pick");

  } else if (pomoState == POMO_RUNNING) {
    unsigned long now     = millis();
    unsigned long elapsed = now - pomoStart;
    unsigned long rem     = (pomoMs > elapsed) ? (pomoMs - elapsed) : 0;
    int mins = (int)(rem / 60000);
    int secs = (int)((rem % 60000) / 1000);

    updateEyes(4, 26, 18, MOOD_NORMAL, 26, 76);
    drawEye(L, true,  MOOD_NORMAL);
    drawEye(R, false, MOOD_NORMAL);
    drawMouth(64, 26, MOOD_NORMAL);

    // Encouragement
    const char* msgs[] = {
      "Focus!", "You got this!", "Stay sharp!",
      "Brain mode!", "Keep going!"
    };
    display.setCursor(4, 32);
    display.print(msgs[(now / 15000) % 5]);

    // Timer (big font)
    display.setFont(&FreeSansBold9pt7b);
    char ts[10];
    sprintf(ts, "%02d:%02d", mins, secs);
    int16_t bx,by; uint16_t bw,bh;
    display.getTextBounds(ts, 0, 0, &bx, &by, &bw, &bh);
    display.setCursor((SCREEN_WIDTH-(int)bw)/2, 48);
    display.print(ts);
    display.setFont(NULL);

    // Progress bar
    int pw = (int)((float)elapsed / (float)pomoMs * 128.0f);
    pw = constrain(pw, 0, 128);
    display.drawRect(0, 58, 128, 5, SSD1306_WHITE);
    display.fillRect(0, 58, pw,  5, SSD1306_WHITE);

    if (rem == 0) {
      pomoState = POMO_DONE;
      pomoDoneAt = now;
      completedSessions++;
    }

  } else {  // POMO_DONE
    unsigned long el = millis() - pomoDoneAt;

    updateEyes(4, 36, 28, MOOD_HAPPY);
    drawEye(L, true,  MOOD_HAPPY);
    drawEye(R, false, MOOD_HAPPY);
    drawMouth(64, 36, MOOD_HAPPY);

    if ((millis() / 500) % 2 == 0)
      display.drawBitmap(56, 0, bmp_heart, 16, 16, SSD1306_WHITE);

    display.setCursor(24, 44);  display.print("Great Job!");
    display.setCursor(24, 54);  display.print("Break Time!");

    // After 3 s auto-return to setup
    if (el > 3000) {
      pomoState    = POMO_SETUP;
      pomoDuration = 5;
      pomoCursor   = 0;
    }
  }

  display.display();
}

// ════════════════════════════════════════════════════
// MOODS SELECTOR
// ════════════════════════════════════════════════════
void drawMoodSelector() {
  int mood = MOOD_LIST[moodSelIdx];
  updateEyes(8, 36, 26, mood);
  display.clearDisplay();
  drawEye(L, true,  mood);
  drawEye(R, false, mood);
  drawMouth(64, 44, mood);

  // Dot row indicator
  int dotX0 = (SCREEN_WIDTH - MOOD_LIST_COUNT * 10) / 2;
  for (int i = 0; i < MOOD_LIST_COUNT; i++) {
    int dx = dotX0 + i * 10 + 4;
    if (i == moodSelIdx) display.fillCircle(dx, 5, 3, SSD1306_WHITE);
    else                 display.drawCircle(dx, 5, 2, SSD1306_WHITE);
  }

  // Mood name
  display.setFont(NULL);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int16_t bx,by; uint16_t bw,bh;
  display.getTextBounds(MOOD_NAMES[moodSelIdx], 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH-(int)bw)/2, 56);
  display.print(MOOD_NAMES[moodSelIdx]);

  display.display();
}

// ════════════════════════════════════════════════════
// VIRTUAL PET  (reused from original)
// ════════════════════════════════════════════════════
void drawPet() {
  unsigned long now = millis();

  // Happiness decay
  if (now - lastPetDecay > 30000) {
    petHappiness -= 1.0f;
    if (petHappiness < 0) petHappiness = 0;
    lastPetDecay = now;
  }

  // Jump animation offset
  float jOff = 0;
  if (isPetJumping) {
    unsigned long el = now - petJumpStart;
    if (el < 300) jOff = -sinf((el / 300.0f) * (float)PI) * 8.0f;
    else isPetJumping = false;
  }

  int petMood = MOOD_NORMAL;
  if      (petHappiness < 30)  petMood = MOOD_SAD;
  else if (petHappiness >= 75) petMood = MOOD_HAPPY;

  updateEyes(12 + jOff, 36, 24, petMood);
  display.clearDisplay();
  drawEye(L, true,  petMood);
  drawEye(R, false, petMood);
  drawMouth(64, 42, petMood);

  if (petMood == MOOD_HAPPY && (now/1000)%2 == 0)
    display.drawBitmap(56, 0, bmp_heart, 16, 16, SSD1306_WHITE);
  else if (petMood == MOOD_SAD && (now/500)%2 == 0)
    display.drawBitmap(56, 0, bmp_anger, 16, 16, SSD1306_WHITE);
  else if (petMood == MOOD_NORMAL && (now/2000)%4 == 0)
    display.drawBitmap(110, 0, bmp_zzz,   16, 16, SSD1306_WHITE);

  display.setFont(NULL);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 54);
  display.print(petHappiness < 30 ? "PET ME! [" : "Happy:  [");
  int fw = (int)((petHappiness / 100.0f) * 45.0f);
  display.drawRect(52, 54, 49, 8, SSD1306_WHITE);
  display.fillRect(54, 56, fw,  4, SSD1306_WHITE);
  display.setCursor(104, 54);
  display.print("]");

  display.display();
}

// ════════════════════════════════════════════════════
// DINO GAME  (unchanged from original)
// ════════════════════════════════════════════════════
void saveDinoHi() {
  prefs.begin("deskbuddy", false);
  prefs.putUInt("dinohi", dinoHi);
  prefs.end();
}

void spawnObs() {
  for (int i = 0; i < 2; i++) {
    if (!obs[i].active) {
      int other = 1 - i;
      if (obs[other].active && obs[other].x > 60.0f) continue;
      obs[i].active = true;
      obs[i].x = 128.0f + (float)random(10, 60);
      obs[i].type = random(0, 3);
      if      (obs[i].type == 0) { obs[i].w=8;  obs[i].h=12; obs[i].y=38; }
      else if (obs[i].type == 1) { obs[i].w=16; obs[i].h=12; obs[i].y=38; }
      else                       { obs[i].w=8;  obs[i].h=6;  obs[i].y=44; }
      break;
    }
  }
}

bool checkColl() {
  int dL=18, dR=28, dT=(int)dinoY+2, dB=(int)dinoY+16;
  for (int i=0; i<2; i++) {
    if (!obs[i].active) continue;
    int oL=(int)obs[i].x, oR=oL+obs[i].w, oT=obs[i].y, oB=oT+obs[i].h;
    if (dR>oL && dL<oR && dB>oT && dT<oB) return true;
  }
  return false;
}

void drawDino() {
  display.clearDisplay();
  unsigned long now = millis();

  if (dinoState == DINO_START) {
    display.setFont(NULL);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(26, 8);  display.print("DESK DINO");
    display.setCursor(14, 22); display.print("Tap = Jump / Start");
    display.setCursor(10, 36); display.print("2x Tap = Main Menu");
    bool bl = (now/300) % 2 == 0;
    display.drawBitmap(56, 46,
      bl ? bmp_dino_run1 : bmp_dino_run2, 16, 16, SSD1306_WHITE);

  } else if (dinoState == DINO_PLAY) {
    // Physics
    if (dinoJumping) {
      dinoVelY += GRAV;
      dinoY    += dinoVelY;
      if (dinoY >= 34.0f) { dinoY=34.0f; dinoJumping=false; dinoVelY=0; }
    }
    spawnObs();
    for (int i=0; i<2; i++) {
      if (obs[i].active) {
        obs[i].x -= gSpeed;
        if (obs[i].x < -(float)obs[i].w) obs[i].active = false;
      }
    }
    dinoScore++;
    if (dinoScore % 150 == 0) { gSpeed += 0.15f; if (gSpeed>6.5f) gSpeed=6.5f; }
    cloudX -= 0.35f;
    if (cloudX < -20.0f) { cloudX=128.0f+(float)random(10,50); cloudY=(float)random(5,20); }
    gndOff -= gSpeed;

    // Cloud
    int cx=(int)cloudX, cy=(int)cloudY;
    display.fillCircle(cx+4,  cy+4, 4, SSD1306_WHITE);
    display.fillCircle(cx+8,  cy+2, 5, SSD1306_WHITE);
    display.fillCircle(cx+12, cy+4, 4, SSD1306_WHITE);
    display.fillRect(cx+2,    cy+4, 12, 4, SSD1306_WHITE);

    // Ground
    display.drawLine(0, 50, 128, 50, SSD1306_WHITE);
    int doff = (int)fabsf(gndOff) % 40;
    for (int x=-doff; x<128; x+=40) {
      if (x >= 0) {
        display.drawLine(x,   52, x+5,  52, SSD1306_WHITE);
        display.drawLine(x+15,55, x+18, 55, SSD1306_WHITE);
      }
    }

    // Dino sprite
    const unsigned char* sp = dinoJumping ? bmp_dino_jump
      : (((now/80)%2==0) ? bmp_dino_run1 : bmp_dino_run2);
    display.drawBitmap(16, (int)dinoY, sp, 16, 16, SSD1306_WHITE);

    // Obstacles
    for (int i=0; i<2; i++) {
      if (!obs[i].active) continue;
      const unsigned char* os =
        (obs[i].type==1) ? bmp_cactus2 :
        (obs[i].type==2) ? bmp_rock    : bmp_cactus1;
      display.drawBitmap((int)obs[i].x, obs[i].y,
                         os, obs[i].w, obs[i].h, SSD1306_WHITE);
    }

    // Score HUD
    display.setFont(NULL);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    char sc[24];
    sprintf(sc, "HI %05d  %05d", dinoHi, (unsigned int)dinoScore);
    display.setCursor(18, 2);
    display.print(sc);

    if (checkColl()) {
      if (dinoScore > dinoHi) {
        dinoHi = (unsigned int)dinoScore;
        saveDinoHi();
        dinoState = DINO_HI;
      } else {
        dinoState = DINO_OVER;
      }
      lastDinoIn = now;
    }

  } else if (dinoState == DINO_OVER) {
    updateEyes(4, 36, 24, MOOD_SAD, 18, 74);
    drawEye(L, true,  MOOD_SAD);
    drawEye(R, false, MOOD_SAD);
    drawMouth(64, 28, MOOD_SAD);
    display.setFont(NULL);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(32, 36); display.print("GAME OVER");
    char sc[30];
    sprintf(sc, "Score:%d  Hi:%d", (int)dinoScore, dinoHi);
    display.setCursor(8, 48);  display.print(sc);
    display.setCursor(8, 58);  display.print("Tap:Retry  2x:Menu");

  } else {  // DINO_HI
    updateEyes(4, 36, 24, MOOD_HAPPY, 18, 74);
    drawEye(L, true,  MOOD_HAPPY);
    drawEye(R, false, MOOD_HAPPY);
    drawMouth(64, 28, MOOD_HAPPY);
    if ((now/1000)%2 == 0)
      display.drawBitmap(56, 0, bmp_heart, 16, 16, SSD1306_WHITE);
    display.setFont(NULL);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(26, 36); display.print("NEW BEST!");
    char sc[20];
    sprintf(sc, "Score: %d", (int)dinoScore);
    display.setCursor(28, 48); display.print(sc);
    display.setCursor(8,  58); display.print("Tap:Retry  2x:Menu");
  }

  display.display();
}

// ════════════════════════════════════════════════════
// SETTINGS
// ════════════════════════════════════════════════════
void drawAbout() {
  display.clearDisplay();
  display.setFont(NULL);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(28, 2);   display.print("DESK BUDDY");
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 14);  display.print("v2.0  Offline");
  display.setCursor(8,  26);  display.print("Edison Sci Corner");
  display.setCursor(26, 38);  display.print("esclabs.in");
  display.setCursor(0,  52);  display.print("Tap or 2x to go back");
  display.display();
}

void drawSettings() {
  display.clearDisplay();
  display.setFont(NULL);
  display.setTextSize(1);

  display.fillRect(0, 0, 128, 11, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  int16_t bx,by; uint16_t bw,bh;
  display.getTextBounds("SETTINGS", 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH-(int)bw)/2, 2);
  display.print("SETTINGS");

  for (int i = 0; i < SETTINGS_COUNT; i++) {
    int y = 12 + i * 13;
    bool sel = (i == settingsCursor);
    if (sel) {
      display.fillRect(0, y, 128, 13, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(4, y+3);
      display.print("> ");
    } else {
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(12, y+3);
    }
    display.print(SETTINGS_ITEMS[i]);
    // Show brightness value inline
    if (i == 0) {
      display.setCursor(92, y+3);
      display.print(highBrightness ? "High" : "Low");
    }
    display.setTextColor(SSD1306_WHITE);
  }

  display.display();
}

// ════════════════════════════════════════════════════
// TAP HANDLERS
// ════════════════════════════════════════════════════
void onSingleTap() {
  switch (appState) {

    case APP_HELLO:
      appState    = APP_MENU;
      menuCursor  = 0;
      lastSaccade = 0;   // immediate gaze movement on entry
      break;

    case APP_MENU:
      menuCursor = (menuCursor + 1) % MENU_COUNT;
      break;

    case APP_CLOCK:
      appState = APP_MENU;
      break;

    case APP_POMODORO:
      if (pomoState == POMO_SETUP) {
        if (pomoCursor == 0) {
          // Increase duration
          pomoDuration += 5;
          if (pomoDuration > 60) pomoDuration = 5;
        } else {
          // Toggle between Start (1) and Back (2)
          pomoCursor = (pomoCursor == 1) ? 2 : 1;
        }
      }
      // No single tap action during RUNNING or DONE
      break;

    case APP_MOODS:
      moodSelIdx = (moodSelIdx + 1) % MOOD_LIST_COUNT;
      break;

    case APP_PET:
      petHappiness += 15.0f;
      if (petHappiness > 100) petHappiness = 100;
      isPetJumping = true;
      petJumpStart = millis();
      break;

    case APP_DINO:
      if (dinoState == DINO_OVER || dinoState == DINO_HI)
        dinoState = DINO_START;   // Retry
      break;

    case APP_SETTINGS:
      if (inAbout) { inAbout = false; break; }
      settingsCursor = (settingsCursor + 1) % SETTINGS_COUNT;
      break;

    default: break;
  }
}

void onDoubleTap() {
  switch (appState) {

    case APP_HELLO:
      appState = APP_MENU;
      menuCursor = 0;
      break;

    case APP_MENU:
      switch (menuCursor) {
        case 0:
          appState = APP_CLOCK;
          break;
        case 1:
          appState     = APP_POMODORO;
          pomoState    = POMO_SETUP;
          pomoDuration = 5;
          pomoCursor   = 0;
          break;
        case 2:
          appState   = APP_MOODS;
          moodSelIdx = 0;
          lastSaccade = 0;
          break;
        case 3:
          appState = APP_PET;
          lastSaccade = 0;
          break;
        case 4:
          appState    = APP_DINO;
          dinoState   = DINO_START;
          dinoScore   = 0;
          gSpeed      = 3.0f;
          dinoY       = 34.0f;
          dinoVelY    = 0;
          dinoJumping = false;
          obs[0].active = obs[1].active = false;
          lastDinoIn  = millis();
          break;
        case 5:
          appState       = APP_SETTINGS;
          settingsCursor = 0;
          inAbout        = false;
          break;
      }
      break;

    case APP_CLOCK:
      appState = APP_MENU;
      break;

    case APP_POMODORO:
      if (pomoState == POMO_SETUP) {
        if (pomoCursor == 0) {
          // Double tap on duration → move focus to Start button
          pomoCursor = 1;
        } else if (pomoCursor == 1) {
          // Start!
          pomoState = POMO_RUNNING;
          pomoStart = millis();
          pomoMs    = (unsigned long)pomoDuration * 60000UL;
        } else {
          // Back / Reset
          pomoDuration = 5;
          pomoCursor   = 0;
          appState     = APP_MENU;
        }
      } else if (pomoState == POMO_RUNNING) {
        // Cancel mid-session → back to setup
        pomoState    = POMO_SETUP;
        pomoDuration = 5;
        pomoCursor   = 0;
        appState     = APP_MENU;
      } else if (pomoState == POMO_DONE) {
        pomoState    = POMO_SETUP;
        pomoDuration = 5;
        pomoCursor   = 0;
        appState     = APP_MENU;
      }
      break;

    case APP_MOODS:
      // Save chosen mood as idle expression
      idleMood = MOOD_LIST[moodSelIdx];
      prefs.begin("deskbuddy", false);
      prefs.putInt("mood", idleMood);
      prefs.end();
      appState = APP_MENU;
      break;

    case APP_PET:
      appState = APP_MENU;
      break;

    case APP_DINO:
      if (dinoState != DINO_PLAY) appState = APP_MENU;
      break;

    case APP_SETTINGS:
      if (inAbout) { inAbout = false; break; }
      switch (settingsCursor) {
        case 0:  // Toggle brightness
          highBrightness = !highBrightness;
          display.ssd1306_command(SSD1306_SETCONTRAST);
          display.ssd1306_command(highBrightness ? 255 : 50);
          prefs.begin("deskbuddy", false);
          prefs.putBool("bright", highBrightness);
          prefs.end();
          break;
        case 1:  // About
          inAbout = true;
          break;
        case 2:  // Reset dino hi-score
          dinoHi = 0;
          prefs.begin("deskbuddy", false);
          prefs.putUInt("dinohi", 0);
          prefs.end();
          break;
        case 3:  // Full reset + reboot
          prefs.begin("deskbuddy", false);
          prefs.clear();
          prefs.end();
          ESP.restart();
          break;
      }
      break;

    default: break;
  }
}

// ════════════════════════════════════════════════════
// TOUCH HANDLER  (single + double tap only)
// ════════════════════════════════════════════════════
void handleTouch() {
  bool pin = digitalRead(TOUCH_PIN);
  unsigned long now = millis();

  if (pin && !lastPinState) {
    pressStart = now;
    longHeld   = false;

    // Instant dino input
    if (appState == APP_DINO) {
      if (dinoState == DINO_START) {
        dinoState   = DINO_PLAY;
        dinoScore   = 0;
        gSpeed      = 3.0f;
        dinoY       = 34.0f;
        dinoVelY    = 0;
        dinoJumping = false;
        obs[0].active = obs[1].active = false;
      } else if (dinoState == DINO_PLAY && !dinoJumping) {
        dinoVelY    = JUMPF;
        dinoJumping = true;
      }
    }
  }

  if (!pin && lastPinState) {
    if (!longHeld) {
      tapCounter++;
      lastTapTime = now;
    }
  }

  lastPinState = pin;

  if (tapCounter > 0 && now - lastTapTime > DOUBLE_TAP_MS) {
    if (tapCounter >= 2) onDoubleTap();
    else                 onSingleTap();
    tapCounter = 0;
  }
}

// ════════════════════════════════════════════════════
// SETUP
// ════════════════════════════════════════════════════
void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  pinMode(TOUCH_PIN, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();

  // Load saved preferences
  prefs.begin("deskbuddy", true);
  dinoHi         = prefs.getUInt("dinohi",  0);
  idleMood       = prefs.getInt ("mood",    MOOD_HAPPY);
  highBrightness = prefs.getBool("bright",  true);
  prefs.end();

  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(highBrightness ? 255 : 50);

  // Initialise eyes very small so they "grow in" during boot
  L.init(18, 30, 2, 2);
  R.init(74, 30, 2, 2);

  appState  = APP_BOOT;
  bootStep  = 0;
  bootStepT = millis();

  lastPetDecay = millis();
}

// ════════════════════════════════════════════════════
// LOOP
// ════════════════════════════════════════════════════
void loop() {
  unsigned long now = millis();

  // Boot animation runs at full frame rate, no touch
  if (appState == APP_BOOT) {
    if (now - lastFrameTime >= FRAME_DELAY) {
      lastFrameTime = now;
      runBoot();
    }
    return;
  }

  handleTouch();

  if (now - lastFrameTime < FRAME_DELAY) return;
  lastFrameTime = now;

  // About sub-screen lives inside Settings
  if (appState == APP_SETTINGS && inAbout) {
    drawAbout();
    return;
  }

  switch (appState) {
    case APP_HELLO:    drawHello();        break;
    case APP_MENU:     drawMenu();         break;
    case APP_CLOCK:    drawClock();        break;
    case APP_POMODORO: drawPomodoro();     break;
    case APP_MOODS:    drawMoodSelector(); break;
    case APP_PET:      drawPet();          break;
    case APP_DINO:     drawDino();         break;
    case APP_SETTINGS: drawSettings();     break;
  }
}
