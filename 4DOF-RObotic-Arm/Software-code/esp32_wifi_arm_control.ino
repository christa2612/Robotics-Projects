#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

static const uint8_t SERVO_COUNT = 4;
static const uint16_t SERVO_MIN_US = 500;
static const uint16_t SERVO_MAX_US = 2400;
static const uint16_t SERVO_CENTER_DEG = 90;
static const uint16_t PWM_FREQUENCY = 50;
static const uint8_t PWM_RESOLUTION_BITS = 16;
static const uint32_t STARTUP_MOVE_DELAY_MS = 2500;
static const uint8_t MG90S_RAMP_STEP_DEG = 4;
static const uint8_t MG90S_RAMP_DELAY_MS = 3;
static const uint8_t MG90S_MEDIUM_RAMP_STEP_DEG = 8;
static const uint8_t MG90S_MEDIUM_RAMP_DELAY_MS = 1;
static const byte DNS_PORT = 53;

// Expansion board wiring:
// P18 -> Clamp, P4 -> Shoulder, P32 -> Elbow, P13 -> Base.
static const uint8_t SERVO_PINS[SERVO_COUNT] = {18, 4, 32, 13};
static const char *JOINT_NAMES[SERVO_COUNT] = {"clamp", "shoulder", "elbow", "gripper"};
static const char *JOINT_LABELS[SERVO_COUNT] = {"Clamp", "Shoulder", "Elbow", "Base"};
static const char *ANGLE_PREF_KEYS[SERVO_COUNT] = {"b_ang", "s_ang", "e_ang", "c_ang"};
static const char *MIN_PREF_KEYS[SERVO_COUNT] = {"b_min", "s_min", "e_min", "c_min"};
static const char *MAX_PREF_KEYS[SERVO_COUNT] = {"b_max", "s_max", "e_max", "c_max"};
static const char *STARTUP_PREF_KEYS[SERVO_COUNT] = {"b_home", "s_home", "e_home", "c_home"};

static const char *PREFERENCES_NAMESPACE = "robot-arm";
static const char *AP_SSID = "RobotArmPro-Setup";
static const char *AP_PASSWORD = "robotarm123";
const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_GATEWAY(192, 168, 4, 1);
const IPAddress AP_SUBNET(255, 255, 255, 0);

struct JointConfig {
  int angle;
  int minAngle;
  int maxAngle;
};

struct DanceStep {
  int angles[SERVO_COUNT];
  uint16_t holdMs;
};

const DanceStep DANCE_STEPS[] = {
  {{65, 120, 70, 35}, 220},
  {{120, 75, 125, 55}, 220},
  {{90, 135, 55, 25}, 200},
  {{55, 80, 120, 60}, 220},
  {{125, 115, 80, 35}, 220},
  {{90, 95, 95, 45}, 260},
  {{70, 125, 65, 20}, 180},
  {{110, 85, 130, 70}, 220},
  {{90, 90, 90, 40}, 280},
};

const char INDEX_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Robot Arm Pro ESP32</title>
    <style>
      :root {
        color-scheme: light;
        --bg: #eef7f4;
        --panel: rgba(255, 255, 255, 0.96);
        --border: rgba(17, 94, 89, 0.16);
        --text: #10201f;
        --muted: #54716e;
        --accent: #0f766e;
        --accent-strong: #0b5f59;
        --accent-soft: rgba(15, 118, 110, 0.12);
        --ok: #15803d;
        --warn: #b42318;
        --shadow: 0 18px 48px rgba(15, 64, 61, 0.14);
      }

      * {
        box-sizing: border-box;
      }

      body {
        margin: 0;
        min-height: 100vh;
        font-family: "Avenir Next", "Segoe UI", sans-serif;
        color: var(--text);
        background:
          radial-gradient(circle at top left, rgba(20, 184, 166, 0.2), transparent 30%),
          linear-gradient(135deg, #e7f5f1 0%, #f8fbff 46%, #e9f2ff 100%);
      }

      body.is-dragging-slider {
        overflow: hidden;
        touch-action: none;
      }

      button,
      input {
        font: inherit;
      }

      .app-shell {
        width: min(1120px, calc(100vw - 28px));
        margin: 14px auto;
        padding: 20px;
        border-radius: 20px;
        border: 1px solid var(--border);
        background: var(--panel);
        box-shadow: var(--shadow);
      }

      .hero {
        display: flex;
        justify-content: space-between;
        gap: 16px;
        align-items: center;
      }

      .eyebrow {
        margin: 0 0 8px;
        font-size: 0.78rem;
        font-weight: 700;
        letter-spacing: 0.16em;
        text-transform: uppercase;
        color: var(--accent-strong);
      }

      h1,
      h2 {
        margin: 0;
      }

      h1 {
        font-size: clamp(1.75rem, 5vw, 3rem);
        line-height: 1;
      }

      .subcopy {
        margin: 10px 0 0;
        max-width: 620px;
        color: var(--muted);
      }

      .hero-card,
      .network-card,
      .joint-card,
      .button-row,
      .note-card {
        border: 1px solid var(--border);
        border-radius: 16px;
        background: rgba(255, 255, 255, 0.76);
      }

      .hero-card {
        min-width: 230px;
        padding: 14px;
      }

      .status-pill {
        display: inline-flex;
        align-items: center;
        justify-content: center;
        padding: 10px 14px;
        border-radius: 999px;
        background: rgba(45, 125, 74, 0.14);
        color: var(--ok);
        font-weight: 700;
      }

      .status-pill.offline {
        background: rgba(163, 60, 33, 0.12);
        color: var(--warn);
      }

      .network-grid,
      .joint-grid {
        margin-top: 12px;
        display: grid;
        gap: 16px;
      }

      .network-grid {
        grid-template-columns: repeat(2, minmax(0, 1fr));
      }

      .network-card,
      .note-card,
      .joint-card {
        padding: 14px;
      }

      .net-title,
      .joint-title {
        font-size: 1.1rem;
        font-weight: 700;
      }

      .net-line,
      .meta-line {
        margin-top: 8px;
        color: var(--muted);
        word-break: break-word;
      }

      .button-row {
        margin-top: 12px;
        padding: 16px;
        display: flex;
        flex-wrap: wrap;
        gap: 12px;
        align-items: center;
      }

      .main-actions {
        backdrop-filter: blur(14px);
      }

      .joint-grid {
        grid-template-columns: repeat(2, minmax(0, 1fr));
      }

      .joint-card-header {
        display: flex;
        justify-content: space-between;
        gap: 12px;
        align-items: baseline;
      }

      .joint-channel {
        color: var(--muted);
        font-size: 0.88rem;
      }

      .angle-output {
        display: inline-flex;
        align-items: center;
        justify-content: center;
        min-width: 62px;
        padding: 8px 10px;
        border-radius: 999px;
        background: var(--accent-soft);
        color: var(--accent-strong);
        font-weight: 700;
      }

      .joint-controls {
        margin-top: 16px;
        display: grid;
        gap: 12px;
      }

      .slider-row {
        display: grid;
        grid-template-columns: 1fr 92px;
        gap: 12px;
        align-items: center;
      }

      .angle-stack {
        display: grid;
        gap: 8px;
      }

      .quick-step-row {
        display: grid;
        grid-template-columns: repeat(4, minmax(0, 1fr));
        gap: 8px;
      }

      .step-button {
        border: 0;
        border-radius: 14px;
        padding: 12px 10px;
        background: rgba(29, 20, 14, 0.06);
        color: var(--text);
        font-weight: 700;
        cursor: pointer;
      }

      .step-button.edge {
        background: rgba(201, 92, 43, 0.14);
        color: var(--accent-strong);
      }

      .limits-panel {
        border: 1px solid rgba(61, 46, 25, 0.1);
        border-radius: 18px;
        background: rgba(255, 255, 255, 0.52);
        overflow: hidden;
      }

      .limits-summary {
        list-style: none;
        padding: 13px 14px;
        font-weight: 700;
        color: var(--muted);
        cursor: pointer;
      }

      .limits-summary::-webkit-details-marker {
        display: none;
      }

      .limits-content {
        padding: 0 14px 14px;
      }

      .limit-row {
        display: grid;
        grid-template-columns: repeat(2, minmax(0, 1fr));
        gap: 12px;
      }

      .field-stack {
        display: grid;
        gap: 6px;
      }

      .field-label {
        font-size: 0.8rem;
        font-weight: 700;
        letter-spacing: 0.05em;
        text-transform: uppercase;
        color: var(--muted);
      }

      input[type="range"] {
        width: 100%;
        accent-color: var(--accent);
        height: 40px;
        background: transparent;
        touch-action: pan-y;
      }

      input[type="range"]::-webkit-slider-runnable-track {
        height: 10px;
        border-radius: 999px;
        background: linear-gradient(90deg, rgba(201, 92, 43, 0.25), rgba(201, 92, 43, 0.7));
      }

      input[type="range"]::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        margin-top: -9px;
        width: 28px;
        height: 28px;
        border-radius: 50%;
        border: 3px solid white;
        background: var(--accent);
        box-shadow: 0 8px 16px rgba(157, 63, 23, 0.25);
      }

      input[type="range"]::-moz-range-track {
        height: 10px;
        border-radius: 999px;
        background: linear-gradient(90deg, rgba(201, 92, 43, 0.25), rgba(201, 92, 43, 0.7));
      }

      input[type="range"]::-moz-range-thumb {
        width: 28px;
        height: 28px;
        border-radius: 50%;
        border: 3px solid white;
        background: var(--accent);
        box-shadow: 0 8px 16px rgba(157, 63, 23, 0.25);
      }

      input[type="number"] {
        width: 100%;
        padding: 10px 12px;
        border-radius: 14px;
        border: 1px solid rgba(61, 46, 25, 0.14);
        background: rgba(255, 255, 255, 0.9);
      }

      .primary-button,
      .secondary-button,
      .action-button {
        border: 0;
        border-radius: 14px;
        padding: 12px 16px;
        cursor: pointer;
        transition: transform 140ms ease, opacity 140ms ease, background 140ms ease;
      }

      .primary-button:hover,
      .secondary-button:hover,
      .action-button:hover {
        transform: translateY(-1px);
      }

      .primary-button {
        background: var(--accent);
        color: white;
        font-weight: 700;
      }

      .secondary-button {
        background: rgba(61, 46, 25, 0.08);
        color: var(--text);
      }

      .action-button {
        background: rgba(29, 20, 14, 0.06);
        color: var(--text);
        font-weight: 600;
      }

      .action-button.is-active {
        background: var(--accent);
        color: white;
      }

      .note-card {
        margin-top: 18px;
        color: var(--muted);
      }

      .note-card strong {
        color: var(--text);
      }

      .live-card {
        margin-top: 12px;
        padding: 14px;
        border: 1px solid var(--border);
        border-radius: 22px;
        background: rgba(255, 255, 255, 0.8);
      }

      .live-header {
        display: flex;
        justify-content: space-between;
        gap: 16px;
        align-items: start;
      }

      .live-subcopy {
        margin: 8px 0 0;
        color: var(--muted);
      }

      .live-chip-row,
      .volume-step-row,
      .volume-button-row,
      .sequence-row {
        margin-top: 14px;
        display: flex;
        flex-wrap: wrap;
        gap: 10px;
        align-items: center;
      }

      .joint-chip,
      .step-toggle {
        border: 0;
        border-radius: 999px;
        padding: 10px 14px;
        font-weight: 700;
        cursor: pointer;
        background: rgba(29, 20, 14, 0.08);
        color: var(--text);
      }

      .joint-chip.is-active,
      .step-toggle.is-active {
        background: var(--accent);
        color: white;
      }

      .live-summary {
        display: inline-flex;
        align-items: center;
        justify-content: center;
        min-width: 120px;
        padding: 10px 14px;
        border-radius: 16px;
        background: var(--accent-soft);
        color: var(--accent-strong);
        font-weight: 700;
        text-align: center;
      }

      .volume-button {
        flex: 1 1 160px;
        min-height: 54px;
        font-size: 1rem;
        font-weight: 700;
      }

      .live-grid {
        margin-top: 16px;
        display: grid;
        gap: 12px;
      }

      .live-row-card {
        border: 1px solid rgba(61, 46, 25, 0.1);
        border-radius: 18px;
        padding: 14px;
        background: rgba(255, 255, 255, 0.7);
      }

      .live-row-card.is-active {
        border-color: rgba(201, 92, 43, 0.38);
        box-shadow: inset 0 0 0 1px rgba(201, 92, 43, 0.14);
      }

      .live-row-header {
        display: flex;
        justify-content: space-between;
        gap: 12px;
        align-items: center;
      }

      .live-row-label {
        border: 0;
        padding: 0;
        background: transparent;
        color: var(--text);
        font-size: 1rem;
        font-weight: 700;
        cursor: pointer;
      }

      .live-row-meta {
        color: var(--muted);
        font-size: 0.88rem;
      }

      .live-row-controls {
        margin-top: 12px;
        display: grid;
        grid-template-columns: 58px 1fr 58px;
        gap: 10px;
        align-items: center;
      }

      .mini-button {
        border: 0;
        border-radius: 14px;
        min-height: 44px;
        font-size: 1.05rem;
        font-weight: 700;
        cursor: pointer;
        background: rgba(29, 20, 14, 0.08);
        color: var(--text);
      }

      .record-stat {
        margin-top: 10px;
        color: var(--muted);
        font-weight: 700;
      }

      .live-topline {
        display: flex;
        justify-content: space-between;
        gap: 14px;
        align-items: start;
      }

      .mini-note {
        margin-top: 10px;
        color: var(--muted);
        font-size: 0.92rem;
      }

      .fader-grid {
        margin-top: 16px;
        display: grid;
        grid-template-columns: repeat(4, minmax(0, 1fr));
        gap: 10px;
        align-items: end;
        padding: 6px 0 12px;
      }

      .fader-column {
        display: grid;
        justify-items: center;
        gap: 10px;
        min-width: 0;
      }

      .fader-readout {
        min-width: 64px;
        padding: 8px 10px;
        border-radius: 999px;
        background: var(--accent-soft);
        color: var(--accent-strong);
        font-weight: 700;
        text-align: center;
      }

      .fader-shell {
        position: relative;
        width: 100%;
        max-width: 74px;
        height: 330px;
        display: flex;
        align-items: center;
        justify-content: center;
      }

      .fader-touch {
        position: relative;
        width: 74px;
        height: 100%;
        touch-action: none;
        user-select: none;
        -webkit-user-select: none;
      }

      .fader-rail {
        position: absolute;
        left: 50%;
        top: 0;
        transform: translateX(-50%);
        width: 18px;
        height: 100%;
        border-radius: 999px;
        background: linear-gradient(180deg, rgba(0, 177, 233, 0.92), rgba(0, 148, 210, 0.98));
        box-shadow:
          inset 0 0 0 3px rgba(255, 255, 255, 0.78),
          0 0 0 1px rgba(29, 20, 14, 0.08);
      }

      .fader-thumb {
        position: absolute;
        left: 50%;
        top: 0;
        width: 44px;
        height: 82px;
        border: 1px solid rgba(0, 0, 0, 0.18);
        border-radius: 14px;
        background:
          linear-gradient(180deg, #6c6c6c 0%, #262626 50%, #595959 100%);
        box-shadow:
          0 10px 18px rgba(0, 0, 0, 0.16),
          inset 0 1px 0 rgba(255, 255, 255, 0.28);
        transform: translateX(-50%);
      }

      .fader-thumb::before,
      .fader-thumb::after {
        content: "";
        position: absolute;
        left: 7px;
        right: 7px;
        height: 2px;
        border-radius: 999px;
        background: rgba(255, 255, 255, 0.3);
      }

      .fader-thumb::before {
        top: 28px;
        box-shadow:
          0 -10px 0 rgba(255, 255, 255, 0.22),
          0 10px 0 rgba(255, 255, 255, 0.22);
      }

      .fader-thumb::after {
        top: 40px;
        box-shadow:
          0 -10px 0 rgba(0, 0, 0, 0.18),
          0 10px 0 rgba(0, 0, 0, 0.18);
      }

      .fader-limit {
        color: var(--muted);
        font-size: 0.82rem;
        font-weight: 700;
      }

      .fader-label {
        font-weight: 700;
        text-align: center;
      }

      .action-cluster {
        margin-top: 12px;
        display: flex;
        flex-wrap: wrap;
        gap: 10px;
      }

      .limits-grid {
        display: grid;
        gap: 12px;
      }

      .limit-item {
        display: grid;
        gap: 10px;
        padding: 12px;
        border-radius: 16px;
        background: rgba(255, 255, 255, 0.68);
      }

      .limit-item-header {
        display: flex;
        justify-content: space-between;
        gap: 12px;
        align-items: center;
      }

      .limit-item-title {
        font-weight: 700;
      }

      .limit-pair {
        display: grid;
        grid-template-columns: repeat(2, minmax(0, 1fr));
        gap: 10px;
      }

      .status-note {
        margin-left: auto;
        font-weight: 700;
        color: var(--muted);
      }

      .status-note.is-dirty {
        color: var(--warn);
      }

      .status-note.is-saved {
        color: var(--ok);
      }

      @media (max-width: 860px) {
        .hero,
        .network-grid,
        .joint-grid {
          grid-template-columns: 1fr;
          display: grid;
        }

        body {
          padding-bottom: 116px;
        }

        .app-shell {
          width: min(100vw - 14px, 1120px);
          margin: 7px auto;
          padding: 16px;
          border-radius: 22px;
        }

        .main-actions {
          position: sticky;
          grid-template-columns: repeat(2, minmax(0, 1fr));
          display: grid;
          top: auto;
          bottom: 10px;
          z-index: 8;
          box-shadow: 0 18px 40px rgba(54, 34, 14, 0.14);
        }

        .main-actions .status-note {
          grid-column: 1 / -1;
        }

        .action-button,
        .primary-button,
        .secondary-button,
        .step-button {
          min-height: 48px;
        }

        .hero-card {
          min-width: 0;
        }

        .joint-card {
          padding: 16px;
        }

        .live-header {
          display: grid;
        }

        .fader-grid {
          grid-template-columns: repeat(4, minmax(0, 1fr));
          gap: 6px;
        }

        .fader-shell {
          max-width: 68px;
          height: 292px;
        }

        .fader-touch {
          width: 68px;
        }

        .fader-thumb {
          width: 40px;
          height: 78px;
        }

        .limit-pair {
          grid-template-columns: 1fr;
        }

        .status-note {
          margin-left: 0;
        }

        .slider-row {
          grid-template-columns: 1fr 82px;
        }

        .limit-row {
          grid-template-columns: 1fr;
        }
      }

      :root {
        --bg: #060B18;
        --panel: rgba(15, 23, 42, 0.76);
        --panel-strong: rgba(17, 24, 39, 0.92);
        --border: rgba(148, 163, 184, 0.18);
        --text: #F8FAFC;
        --muted: #94A3B8;
        --accent: #00D4FF;
        --accent-strong: #67E8F9;
        --accent-soft: rgba(0, 212, 255, 0.12);
        --ok: #00E676;
        --warn: #F59E0B;
        --danger: #EF4444;
        --violet: #7C4DFF;
        --shadow: 0 24px 80px rgba(0, 0, 0, 0.42);
      }

      body {
        background:
          radial-gradient(circle at 12% 4%, rgba(0, 212, 255, 0.34), transparent 31%),
          radial-gradient(circle at 86% 10%, rgba(0, 230, 118, 0.24), transparent 30%),
          radial-gradient(circle at 58% 106%, rgba(14, 165, 233, 0.2), transparent 32%),
          linear-gradient(145deg, #04131F 0%, #08253A 42%, #06111F 100%);
        letter-spacing: 0;
      }

      .app-shell {
        background:
          radial-gradient(circle at 0% 0%, rgba(0, 212, 255, 0.08), transparent 36%),
          linear-gradient(180deg, rgba(8, 34, 52, 0.86), rgba(4, 13, 25, 0.76));
        border-color: rgba(125, 211, 252, 0.18);
        box-shadow: var(--shadow), inset 0 1px 0 rgba(255, 255, 255, 0.05);
        backdrop-filter: blur(18px);
      }

      .hero {
        padding: 4px 2px 8px;
      }

      .eyebrow {
        color: var(--accent-strong);
      }

      .subcopy,
      .meta-line,
      .net-line,
      .live-subcopy,
      .mini-note,
      .record-stat {
        color: var(--muted);
      }

      .hero-card,
      .network-card,
      .live-card,
      .limits-panel,
      .limit-item {
        background:
          radial-gradient(circle at 18% 0%, rgba(0, 212, 255, 0.08), transparent 40%),
          linear-gradient(180deg, rgba(30, 41, 59, 0.82), rgba(15, 23, 42, 0.74));
        border-color: rgba(125, 211, 252, 0.16);
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.06);
        backdrop-filter: blur(18px);
      }

      .status-pill,
      .live-summary {
        background: rgba(0, 230, 118, 0.12);
        border: 1px solid rgba(0, 230, 118, 0.36);
        color: #86EFAC;
        box-shadow: 0 0 24px rgba(0, 230, 118, 0.14);
      }

      .status-pill.offline,
      .live-summary.offline {
        background: rgba(239, 68, 68, 0.12);
        border-color: rgba(239, 68, 68, 0.36);
        color: #FCA5A5;
      }

      .network-grid {
        grid-template-columns: repeat(4, minmax(0, 1fr));
      }

      .network-card {
        min-height: 96px;
      }

      .net-title {
        color: var(--text);
        font-size: 0.78rem;
        letter-spacing: 0.08em;
        text-transform: uppercase;
      }

      .net-line {
        font-size: 0.92rem;
      }

      .robot-console {
        display: grid;
        gap: 16px;
      }

      .visual-panel {
        position: relative;
        min-height: 294px;
        overflow: hidden;
        border: 1px solid rgba(125, 211, 252, 0.2);
        border-radius: 24px;
        background:
          radial-gradient(circle at 20% 12%, rgba(0, 212, 255, 0.28), transparent 30%),
          radial-gradient(circle at 82% 18%, rgba(0, 230, 118, 0.16), transparent 32%),
          linear-gradient(135deg, rgba(8, 31, 49, 0.98), rgba(2, 10, 23, 0.94) 58%, rgba(6, 26, 39, 0.98));
        box-shadow:
          inset 0 1px 0 rgba(255,255,255,0.08),
          inset 0 -1px 0 rgba(0, 212, 255, 0.1),
          0 28px 76px rgba(0,0,0,0.28);
      }

      .visual-header {
        position: relative;
        z-index: 2;
        display: flex;
        justify-content: space-between;
        gap: 12px;
        padding: 18px 18px 0;
      }

      .section-kicker {
        color: var(--accent-strong);
        font-size: 0.74rem;
        font-weight: 800;
        letter-spacing: 0.12em;
        text-transform: uppercase;
      }

      .section-title {
        margin-top: 4px;
        color: var(--text);
        font-size: 1.18rem;
        font-weight: 800;
      }

      .robot-stage {
        position: absolute;
        inset: 0;
      }

      .robot-stage::before {
        content: "";
        position: absolute;
        inset: 72px 22px 20px;
        border-radius: 26px;
        background:
          linear-gradient(90deg, rgba(0, 212, 255, 0.08), transparent 18%, transparent 82%, rgba(124, 77, 255, 0.1)),
          repeating-linear-gradient(90deg, rgba(148, 163, 184, 0.06) 0 1px, transparent 1px 44px);
        mask-image: linear-gradient(to bottom, transparent, black 16%, black 86%, transparent);
      }

      .grid-floor {
        position: absolute;
        left: 8%;
        right: 8%;
        bottom: 22px;
        height: 92px;
        opacity: 0.38;
        background:
          linear-gradient(rgba(0,212,255,0.22) 1px, transparent 1px),
          linear-gradient(90deg, rgba(0,212,255,0.22) 1px, transparent 1px);
        background-size: 26px 20px;
        transform: perspective(320px) rotateX(60deg);
        transform-origin: center bottom;
      }

      .arm-svg {
        position: absolute;
        left: 50%;
        bottom: 12px;
        width: min(500px, 92vw);
        height: 194px;
        transform: translateX(-50%);
        overflow: visible;
      }

      .arm-link,
      .arm-joint,
      .arm-claw {
        transition: opacity 180ms ease;
      }

      .arm-segment {
        transform-origin: 0 0;
      }

      .arm-claw {
        transform-box: fill-box;
        transform-origin: 0 0;
      }

      .arm-link {
        stroke: url(#armGradient);
        stroke-width: 18;
        stroke-linecap: round;
        filter: drop-shadow(0 0 12px rgba(0, 212, 255, 0.28));
      }

      .arm-joint {
        fill: url(#jointGradient);
        stroke: #00D4FF;
        stroke-width: 3;
        filter: drop-shadow(0 0 12px rgba(0, 212, 255, 0.44));
      }

      .arm-base-platform {
        fill: url(#baseGradient);
        stroke: rgba(125, 211, 252, 0.42);
        stroke-width: 2;
        filter: drop-shadow(0 16px 22px rgba(0, 0, 0, 0.28));
      }

      .arm-base-glow {
        fill: rgba(0, 212, 255, 0.18);
        filter: blur(1px);
      }

      .arm-housing {
        fill: rgba(15, 23, 42, 0.96);
        stroke: rgba(226, 232, 240, 0.32);
        stroke-width: 2;
      }

      .arm-wrist {
        fill: rgba(226, 232, 240, 0.94);
        stroke: rgba(0, 212, 255, 0.7);
        stroke-width: 2;
        filter: drop-shadow(0 0 10px rgba(0, 212, 255, 0.32));
      }

      .claw-finger {
        fill: none;
        stroke: #E0F2FE;
        stroke-width: 7;
        stroke-linecap: round;
        stroke-linejoin: round;
        filter: drop-shadow(0 0 8px rgba(0, 212, 255, 0.36));
      }

      .knob-panel {
        display: grid;
        gap: 14px;
      }

      .knob-grid {
        display: grid;
        grid-template-columns: repeat(2, minmax(0, 1fr));
        gap: 12px;
      }

      .knob-card {
        display: grid;
        justify-items: center;
        gap: 10px;
        min-height: 218px;
        padding: 16px 12px;
        border: 1px solid rgba(125, 211, 252, 0.16);
        border-radius: 22px;
        background:
          radial-gradient(circle at 50% 0%, rgba(0, 212, 255, 0.11), transparent 42%),
          linear-gradient(180deg, rgba(15, 23, 42, 0.96), rgba(2, 6, 23, 0.76));
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.05), 0 16px 36px rgba(0, 0, 0, 0.16);
        transition: transform 180ms ease, border-color 180ms ease, box-shadow 180ms ease;
      }

      .knob-card:hover {
        transform: translateY(-2px);
        border-color: rgba(0, 212, 255, 0.34);
        box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.07), 0 20px 46px rgba(0, 0, 0, 0.22);
      }

      .rotary-knob {
        --angle: 90deg;
        --progress: 50%;
        position: relative;
        width: min(158px, 40vw);
        aspect-ratio: 1;
        border: 0;
        border-radius: 50%;
        cursor: grab;
        touch-action: none;
        user-select: none;
        color: var(--text);
        background:
          radial-gradient(circle at 50% 50%, #101A32 0 36%, transparent 37%),
          radial-gradient(circle at 32% 24%, rgba(255,255,255,0.18), transparent 22%),
          conic-gradient(from 225deg, #00D4FF 0 var(--progress), #7C4DFF var(--progress), rgba(148,163,184,0.14) var(--progress) 75%, transparent 75% 100%),
          linear-gradient(145deg, rgba(30,41,59,0.98), rgba(2,6,23,0.96));
        box-shadow:
          inset 0 0 0 1px rgba(148, 163, 184, 0.18),
          inset 0 12px 24px rgba(255, 255, 255, 0.04),
          inset 0 -18px 28px rgba(0, 0, 0, 0.28),
          0 20px 46px rgba(0, 0, 0, 0.34);
        transition: box-shadow 180ms ease, transform 160ms ease;
      }

      .rotary-knob::before {
        content: "";
        position: absolute;
        inset: 9px;
        border-radius: 50%;
        border: 1px solid rgba(226, 232, 240, 0.12);
        background:
          repeating-conic-gradient(from 225deg, rgba(226,232,240,0.36) 0deg 1deg, transparent 1deg 9deg);
        mask-image: radial-gradient(circle, transparent 0 63%, black 64% 74%, transparent 75%);
        opacity: 0.7;
      }

      .rotary-knob:active {
        cursor: grabbing;
        transform: scale(0.99);
      }

      .rotary-knob.is-active {
        box-shadow:
          inset 0 0 0 1px rgba(0, 212, 255, 0.5),
          0 0 28px rgba(0, 212, 255, 0.22),
          0 18px 42px rgba(0, 0, 0, 0.34);
      }

      .knob-indicator {
        position: absolute;
        inset: 12px;
        border-radius: 50%;
        transform: rotate(var(--angle));
        transition: transform 180ms cubic-bezier(.2,.8,.2,1);
      }

      .knob-indicator::before {
        content: "";
        position: absolute;
        left: 50%;
        top: 2px;
        width: 5px;
        height: 28px;
        border-radius: 999px;
        background: linear-gradient(180deg, #FFFFFF, #00D4FF);
        box-shadow: 0 0 18px rgba(0,212,255,0.74), 0 0 30px rgba(124,77,255,0.24);
        transform: translateX(-50%);
      }

      .knob-center {
        position: absolute;
        inset: 40px;
        display: grid;
        place-content: center;
        border-radius: 50%;
        background:
          radial-gradient(circle at 50% 28%, rgba(59, 130, 246, 0.18), transparent 36%),
          radial-gradient(circle at 50% 50%, rgba(30,41,59,0.98), rgba(2,6,23,0.96));
        border: 1px solid rgba(125, 211, 252, 0.2);
        box-shadow: inset 0 1px 10px rgba(255,255,255,0.05);
      }

      .knob-value {
        color: var(--text);
        font-size: 1.28rem;
        font-weight: 850;
        font-variant-numeric: tabular-nums;
      }

      .knob-range {
        color: var(--muted);
        font-size: 0.74rem;
        font-weight: 700;
      }

      .knob-label {
        color: var(--text);
        font-size: 1rem;
        font-weight: 800;
        text-transform: uppercase;
        letter-spacing: 0.06em;
      }

      .action-section {
        display: grid;
        gap: 10px;
        padding: 14px;
        border: 1px solid rgba(148, 163, 184, 0.16);
        border-radius: 20px;
        background: rgba(15, 23, 42, 0.74);
      }

      .action-section-title {
        color: var(--muted);
        font-size: 0.74rem;
        font-weight: 800;
        letter-spacing: 0.12em;
        text-transform: uppercase;
      }

      .action-cluster {
        margin-top: 0;
      }

      .primary-button,
      .secondary-button,
      .action-button,
      .mini-button,
      .step-button {
        min-height: 46px;
        border: 1px solid rgba(56, 189, 248, 0.28);
        border-radius: 14px;
        background:
          linear-gradient(135deg, rgba(14, 165, 233, 0.18), rgba(124, 77, 255, 0.12)),
          rgba(15, 23, 42, 0.9);
        color: #EAF6FF;
        box-shadow: inset 0 1px 0 rgba(255,255,255,0.05), 0 10px 24px rgba(2, 6, 23, 0.18);
        transition: transform 160ms ease, box-shadow 180ms ease, border-color 180ms ease, background 180ms ease;
      }

      .button-green {
        border-color: rgba(0, 230, 118, 0.34);
        background: linear-gradient(135deg, rgba(0, 230, 118, 0.24), rgba(20, 184, 166, 0.14)), rgba(15, 23, 42, 0.9);
      }

      .button-blue {
        border-color: rgba(0, 212, 255, 0.36);
        background: linear-gradient(135deg, rgba(0, 212, 255, 0.24), rgba(59, 130, 246, 0.14)), rgba(15, 23, 42, 0.9);
      }

      .button-violet {
        border-color: rgba(124, 77, 255, 0.38);
        background: linear-gradient(135deg, rgba(124, 77, 255, 0.24), rgba(0, 212, 255, 0.1)), rgba(15, 23, 42, 0.9);
      }

      .button-amber {
        border-color: rgba(245, 158, 11, 0.4);
        background: linear-gradient(135deg, rgba(245, 158, 11, 0.24), rgba(251, 191, 36, 0.1)), rgba(15, 23, 42, 0.9);
      }

      .button-red {
        border-color: rgba(248, 113, 113, 0.38);
        background: linear-gradient(135deg, rgba(239, 68, 68, 0.24), rgba(244, 63, 94, 0.1)), rgba(15, 23, 42, 0.9);
      }

      .primary-button:hover,
      .secondary-button:hover,
      .action-button:hover,
      .mini-button:hover,
      .step-button:hover {
        transform: translateY(-1px);
        border-color: rgba(0, 212, 255, 0.48);
        background:
          linear-gradient(135deg, rgba(0, 212, 255, 0.24), rgba(124, 77, 255, 0.18)),
          rgba(15, 23, 42, 0.94);
        box-shadow: inset 0 1px 0 rgba(255,255,255,0.07), 0 14px 30px rgba(0, 0, 0, 0.24);
      }

      .primary-button {
        background: linear-gradient(135deg, #00D4FF, #7C4DFF);
        color: #020617;
        border-color: transparent;
        box-shadow: 0 0 28px rgba(0, 212, 255, 0.22);
      }

      .action-button.is-active {
        background: rgba(0, 212, 255, 0.14);
        border-color: rgba(0, 212, 255, 0.58);
        color: #67E8F9;
        box-shadow: 0 0 22px rgba(0, 212, 255, 0.18);
      }

      input[type="number"] {
        background: #FFFFFF;
        border-color: rgba(0, 212, 255, 0.42);
        color: #06111F;
        box-shadow: inset 0 1px 0 rgba(255,255,255,0.7), 0 8px 18px rgba(0, 0, 0, 0.16);
      }

      .limits-summary,
      .field-label,
      .live-row-meta {
        color: var(--muted);
      }

      @media (max-width: 700px) {
        .app-shell {
          width: min(100vw - 16px, 520px);
          margin: 8px auto;
          padding: 12px;
          border-radius: 18px;
        }

        .hero {
          align-items: flex-start;
          gap: 10px;
        }

        .hero-card {
          min-width: 128px;
        }

        .network-grid {
          grid-template-columns: repeat(2, minmax(0, 1fr));
          gap: 8px;
        }

        .network-card {
          min-height: 82px;
          padding: 12px;
        }

        .visual-panel {
          min-height: 188px;
          border-radius: 20px;
        }

        .visual-header {
          padding: 12px 12px 0;
          align-items: flex-start;
        }

        .section-title {
          font-size: 1rem;
        }

        .section-kicker {
          font-size: 0.66rem;
        }

        .live-summary {
          padding: 7px 9px;
          font-size: 0.72rem;
        }

        .robot-stage::before {
          inset: 58px 10px 10px;
          border-radius: 18px;
        }

        .grid-floor {
          left: 4%;
          right: 4%;
          bottom: 12px;
          height: 60px;
          background-size: 20px 16px;
          transform: perspective(260px) rotateX(58deg);
        }

        .arm-svg {
          bottom: 8px;
          width: min(340px, 102vw);
          height: 132px;
        }

        .arm-link {
          stroke-width: 15;
        }

        .arm-joint {
          stroke-width: 2.4;
        }

        .claw-finger {
          stroke-width: 6;
        }

        .knob-grid {
          gap: 10px;
        }

        .knob-card {
          min-height: 184px;
          padding: 14px 8px;
        }

        .rotary-knob {
          width: min(134px, 40vw);
        }
      }
    </style>
  </head>
  <body>
    <main class="app-shell">
      <section class="hero">
        <div>
          <p class="eyebrow">Industrial Robotics Console</p>
          <h1>Robot Arm Pro</h1>
          <p class="subcopy">
            Mission-ready control for the Nano32 robotic arm.
          </p>
        </div>

        <div class="hero-card">
          <div class="status-pill" id="deviceStatus">Loading…</div>
          <div class="meta-line" id="deviceMeta">Waiting for ESP32 status</div>
        </div>
      </section>

      <section class="network-grid">
        <article class="network-card">
          <div class="net-title">Connection</div>
          <div class="net-line" id="connectionStatus">CONNECTED</div>
        </article>
        <article class="network-card">
          <div class="net-title">Servo GPIO</div>
          <div class="net-line" id="pcaStatus">Checking...</div>
        </article>
        <article class="network-card">
          <div class="net-title">Live Control</div>
          <div class="net-line" id="liveControlStatus">ACTIVE</div>
        </article>
        <article class="network-card">
          <div class="net-title">Network</div>
          <div class="net-line" id="apSsid">SSID: -</div>
          <div class="net-line" id="apIp">IP: -</div>
          <div class="net-line" id="phoneIp">Phone IP: -</div>
        </article>
      </section>

      <section class="live-card" id="liveControlCard"></section>
    </main>

    <script>
      const liveControlCard = document.getElementById("liveControlCard");

      const RECORDING_STORAGE_KEY = "robotArmProSequenceV1";
      const LOOP_STORAGE_KEY = "robotArmProLoopV1";
      const RECORD_FRAME_MIN_MS = 90;

      let liveStatus = null;
      let draftSettings = {};
      let draftDirty = false;
      let refreshInterval = null;
      let statusNote = null;
      let statusMessage = "Loading ESP32 settings...";
      let statusClassName = "";
      const moveTimers = {};
      const moveQueuedAngles = {};
      const moveInFlight = {};
      const lastMoveSentAt = {};
      let recordedFrames = [];
      let isRecording = false;
      let recordStartedAt = 0;
      let lastRecordedAt = 0;
      let isPlayingSequence = false;
      let playbackRunId = 0;
      let playbackTimer = null;
      let loopPlayback = false;
      let limitsExpanded = false;
      let activeFaderKey = "";

      function clamp(value, min, max) {
        return Math.max(min, Math.min(max, value));
      }

      function toForm(data) {
        const form = new URLSearchParams();
        Object.entries(data).forEach(([key, value]) => {
          form.set(key, String(value));
        });
        return form;
      }

      async function api(path, options = {}) {
        const response = await fetch(path, {
          cache: "no-store",
          ...options,
        });

        if (!response.ok) {
          const message = await response.text();
          throw new Error(message || `HTTP ${response.status}`);
        }

        const contentType = response.headers.get("content-type") || "";
        if (contentType.includes("application/json")) {
          return response.json();
        }

        return response.text();
      }

      function getPoseSnapshot() {
        const pose = {};
        Object.entries(draftSettings).forEach(([jointKey, joint]) => {
          pose[jointKey] = joint.angle;
        });
        return pose;
      }

      function getPoseFromFrame(frame) {
        const pose = {};
        Object.entries(draftSettings).forEach(([jointKey, joint]) => {
          const rawValue = Number.parseInt(frame?.pose?.[jointKey], 10);
          pose[jointKey] = Number.isInteger(rawValue) ? clamp(rawValue, joint.min, joint.max) : joint.angle;
        });
        return pose;
      }

      function samePose(firstPose, secondPose) {
        return Object.keys(draftSettings).every((jointKey) => Number(firstPose?.[jointKey]) === Number(secondPose?.[jointKey]));
      }

      function saveSequenceState() {
        try {
          localStorage.setItem(RECORDING_STORAGE_KEY, JSON.stringify(recordedFrames));
          localStorage.setItem(LOOP_STORAGE_KEY, loopPlayback ? "1" : "0");
        } catch (error) {
          // Ignore storage errors inside captive-portal webviews.
        }
      }

      function loadSequenceState() {
        try {
          const savedFrames = JSON.parse(localStorage.getItem(RECORDING_STORAGE_KEY) || "[]");
          recordedFrames = Array.isArray(savedFrames)
            ? savedFrames.slice(0, 240).map((frame) => ({
                at: Number.isFinite(Number(frame?.at)) ? Math.max(0, Number(frame.at)) : 0,
                pose: frame && typeof frame.pose === "object" ? frame.pose : {},
              }))
            : [];
          loopPlayback = localStorage.getItem(LOOP_STORAGE_KEY) === "1";
        } catch (error) {
          recordedFrames = [];
          loopPlayback = false;
        }
      }

      function copyDraftFromStatus(status) {
        draftSettings = {};
        status.joints.forEach((joint) => {
          draftSettings[joint.key] = {
            angle: joint.angle,
            min: joint.min,
            max: joint.max,
            startupAngle: joint.startupAngle,
            label: joint.label,
            channel: joint.channel,
          };
        });
      }

      function applyStatusNoteToDom() {
        if (!statusNote) {
          return;
        }

        statusNote.textContent = statusMessage;
        statusNote.classList.remove("is-dirty", "is-saved");
        if (statusClassName) {
          statusNote.classList.add(statusClassName);
        }
      }

      function setStatusNote(message, className = "") {
        statusMessage = message;
        statusClassName = className;
        applyStatusNoteToDom();
      }

      function markDirty(message = "Unsaved changes on ESP32") {
        draftDirty = true;
        setStatusNote(message, "is-dirty");
      }

      function markSaved(message = "ESP32 settings saved") {
        draftDirty = false;
        setStatusNote(message, "is-saved");
      }

      function setSliderDragState(active) {
        document.body.classList.toggle("is-dragging-slider", active);
      }

      function getEventPoint(event) {
        if (event.touches && event.touches.length) {
          return { x: event.touches[0].clientX, y: event.touches[0].clientY };
        }

        if (event.changedTouches && event.changedTouches.length) {
          return { x: event.changedTouches[0].clientX, y: event.changedTouches[0].clientY };
        }

        if (typeof event.clientX === "number" && typeof event.clientY === "number") {
          return { x: event.clientX, y: event.clientY };
        }

        return null;
      }

      function angleToKnobRotation(joint) {
        const ratio = joint.max === joint.min ? 0 : ((joint.angle - joint.min) / (joint.max - joint.min));
        return -135 + (ratio * 270);
      }

      function angleToKnobProgress(joint) {
        const ratio = joint.max === joint.min ? 0 : ((joint.angle - joint.min) / (joint.max - joint.min));
        return `${Math.round(ratio * 75)}%`;
      }

      function updateKnobFromEvent(jointKey, event) {
        const joint = draftSettings[jointKey];
        const surface = document.querySelector(`[data-knob="${jointKey}"]`);
        const point = getEventPoint(event);

        if (!joint || !surface || !point) {
          return;
        }

        const rect = surface.getBoundingClientRect();
        const centerX = rect.left + rect.width / 2;
        const centerY = rect.top + rect.height / 2;
        const dx = point.x - centerX;
        const dy = point.y - centerY;
        let degrees = Math.atan2(dy, dx) * 180 / Math.PI + 90;
        if (degrees < -180) {
          degrees += 360;
        }
        if (degrees > 180) {
          degrees -= 360;
        }

        const rotation = clamp(degrees, -135, 135);
        let ratio = (rotation + 135) / 270;
        if (ratio < 0.025) {
          ratio = 0;
        }
        if (ratio > 0.975) {
          ratio = 1;
        }
        const nextAngle = Math.round(joint.min + (ratio * (joint.max - joint.min)));

        updateDraftAngle(jointKey, nextAngle, true, "manual");
      }

      function startFaderDrag(jointKey, event) {
        activeFaderKey = jointKey;
        setSliderDragState(true);
        const surface = document.querySelector(`[data-knob="${jointKey}"]`);
        if (surface) {
          surface.classList.add("is-active");
        }

        if (event && event.cancelable) {
          event.preventDefault();
        }

        updateKnobFromEvent(jointKey, event);
      }

      function moveActiveFader(event) {
        if (!activeFaderKey) {
          return;
        }

        if (event && event.cancelable) {
          event.preventDefault();
        }

        updateKnobFromEvent(activeFaderKey, event);
      }

      function endActiveFader() {
        if (activeFaderKey) {
          const surface = document.querySelector(`[data-knob="${activeFaderKey}"]`);
          if (surface) {
            surface.classList.remove("is-active");
          }
        }
        activeFaderKey = "";
        setSliderDragState(false);
      }

      function bindKnobInteraction(surface, jointKey) {
        surface.addEventListener("mousedown", (event) => {
          startFaderDrag(jointKey, event);
        });

        surface.addEventListener("touchstart", (event) => {
          startFaderDrag(jointKey, event);
        }, { passive: false });
      }

      function updateRobotVisualization() {
        const base = draftSettings.gripper?.angle ?? 90;
        const shoulder = draftSettings.shoulder?.angle ?? 90;
        const elbow = draftSettings.elbow?.angle ?? 90;
        const clamp = draftSettings.clamp?.angle ?? 45;
        const baseEl = document.querySelector("[data-arm-base]");
        const shoulderEl = document.querySelector("[data-arm-shoulder]");
        const elbowEl = document.querySelector("[data-arm-elbow]");
        const clampEl = document.querySelector("[data-arm-clamp]");

        if (baseEl) {
          baseEl.setAttribute("transform", `translate(0 -38) rotate(${(base - 90) * 0.32})`);
        }
        if (shoulderEl) {
          shoulderEl.setAttribute("transform", `translate(78 -44) rotate(${(shoulder - 90) * -0.42})`);
        }
        if (elbowEl) {
          elbowEl.setAttribute("transform", `translate(82 -20) rotate(${(elbow - 90) * 0.48})`);
        }
        if (clampEl) {
          const clawAngle = 10 + (clamp / 180) * 24;
          clampEl.setAttribute("transform", "translate(58 12)");
          const upperClaw = clampEl.querySelector("[data-claw-upper]");
          const lowerClaw = clampEl.querySelector("[data-claw-lower]");
          if (upperClaw) {
            upperClaw.setAttribute("transform", `rotate(${-clawAngle})`);
          }
          if (lowerClaw) {
            lowerClaw.setAttribute("transform", `rotate(${clawAngle})`);
          }
        }
      }

      function controllerBadgeText(status) {
        if (!status?.pcaReady) {
          return "Servo PWM Offline";
        }

        if (isRecording) {
          return "Recording";
        }

        if (isPlayingSequence) {
          return loopPlayback ? "Looping" : "Playing";
        }

        return "Ready";
      }

      function updateNetworkCards(status) {
        const deviceStatus = document.getElementById("deviceStatus");
        const deviceMeta = document.getElementById("deviceMeta");

        deviceStatus.textContent = controllerBadgeText(status);
        deviceStatus.classList.toggle("offline", !status.pcaReady);

        if (!status.pcaReady) {
          deviceMeta.textContent = `Hotspot ready at ${status.apIp}. Check expansion board servo power and GPIO wiring.`;
        } else {
          deviceMeta.textContent = `Open ${status.apIp} and control the robot live.`;
        }

        document.getElementById("connectionStatus").textContent = "CONNECTED";
        document.getElementById("pcaStatus").textContent = status.pcaReady ? "GPIO PWM ONLINE" : "OFFLINE";
        document.getElementById("liveControlStatus").textContent = status.danceActive ? "SEQUENCE" : "ACTIVE";
        document.getElementById("apSsid").textContent = `SSID: ${status.apSsid}`;
        document.getElementById("apIp").textContent = `IP: ${status.apIp}`;
        document.getElementById("phoneIp").textContent = `Phone IP: ${status.clientIp || "-"}`;
      }

      function sequenceSummaryText() {
        const count = recordedFrames.length;
        const label = count === 1 ? "frame" : "frames";

        if (isRecording) {
          return `Recording live move · ${count} ${label}`;
        }

        if (isPlayingSequence) {
          return `Playing ${count} ${label}${loopPlayback ? " in loop" : ""}`;
        }

        if (!count) {
          return "No recorded move saved";
        }

        return `${count} ${label} saved${loopPlayback ? " · Loop On" : ""}`;
      }

      function selectedSpeedMode() {
        return liveStatus?.speedMode || "slow";
      }

      function renderLiveControlCard() {
        const joints = Object.entries(draftSettings);
        if (!joints.length) {
          liveControlCard.innerHTML = "";
          statusNote = null;
          return;
        }

        liveControlCard.innerHTML = `
          <div class="robot-console">
            <section class="visual-panel">
              <div class="visual-header">
                <div>
                  <div class="section-kicker">Robot Visualization</div>
                  <div class="section-title">Real-time Pose</div>
                </div>
                <div class="live-summary" id="controllerBadge">${controllerBadgeText(liveStatus)}</div>
              </div>
              <div class="robot-stage">
                <div class="grid-floor"></div>
                <svg class="arm-svg" viewBox="0 0 420 240" aria-label="Robotic arm pose visualization">
                  <defs>
                    <linearGradient id="armGradient" x1="0" y1="0" x2="1" y2="1">
                      <stop offset="0" stop-color="#E0F2FE"/>
                      <stop offset="0.46" stop-color="#00D4FF"/>
                      <stop offset="1" stop-color="#7C4DFF"/>
                    </linearGradient>
                    <linearGradient id="jointGradient" x1="0" y1="0" x2="1" y2="1">
                      <stop offset="0" stop-color="#1E293B"/>
                      <stop offset="0.55" stop-color="#020617"/>
                      <stop offset="1" stop-color="#0F172A"/>
                    </linearGradient>
                    <linearGradient id="baseGradient" x1="0" y1="0" x2="1" y2="1">
                      <stop offset="0" stop-color="#1E293B"/>
                      <stop offset="0.5" stop-color="#0F172A"/>
                      <stop offset="1" stop-color="#020617"/>
                    </linearGradient>
                  </defs>
                  <g transform="translate(78 204)">
                    <ellipse class="arm-base-glow" cx="0" cy="16" rx="76" ry="15"/>
                    <path class="arm-base-platform" d="M-52 8 C-42 -14 42 -14 52 8 L68 27 C48 44 -48 44 -68 27 Z"/>
                    <rect class="arm-housing" x="-26" y="-34" width="52" height="50" rx="16"/>
                    <circle class="arm-joint" cx="0" cy="-38" r="19"/>
                    <g class="arm-segment" data-arm-base transform="translate(0 -38)">
                      <line class="arm-link" x1="0" y1="0" x2="78" y2="-44"/>
                      <circle class="arm-joint" cx="78" cy="-44" r="16"/>
                      <g class="arm-segment" data-arm-shoulder transform="translate(78 -44)">
                        <line class="arm-link" x1="0" y1="0" x2="82" y2="-20"/>
                        <circle class="arm-joint" cx="82" cy="-20" r="14"/>
                        <g class="arm-segment" data-arm-elbow transform="translate(82 -20)">
                          <line class="arm-link" x1="0" y1="0" x2="58" y2="12"/>
                          <circle class="arm-wrist" cx="58" cy="12" r="12"/>
                          <g class="arm-claw" data-arm-clamp transform="translate(58 12)">
                            <g data-claw-upper>
                              <path class="claw-finger" d="M0 0 L28 -12 L48 -8"/>
                            </g>
                            <g data-claw-lower>
                              <path class="claw-finger" d="M0 0 L28 12 L48 8"/>
                            </g>
                            <circle class="arm-wrist" cx="0" cy="0" r="7"/>
                          </g>
                        </g>
                      </g>
                    </g>
                  </g>
                </svg>
              </div>
            </section>

            <section class="knob-panel">
              <div class="live-topline">
                <div>
                  <div class="section-kicker">Servo Controls</div>
                  <div class="section-title">Rotary Angle Control</div>
                  <p class="live-subcopy">Drag each knob. Existing live-control behavior and servo updates are preserved.</p>
                </div>
              </div>

              <div class="action-section">
                <div class="action-section-title">Command Mode</div>
                <div class="action-cluster">
                  <button type="button" class="action-button button-green${selectedSpeedMode() === "slow" ? " is-active" : ""}" data-speed="slow">Slow</button>
                  <button type="button" class="action-button button-blue${selectedSpeedMode() === "medium" ? " is-active" : ""}" data-speed="medium">Medium</button>
                  <button type="button" class="action-button button-violet${selectedSpeedMode() === "fast" ? " is-active" : ""}" data-speed="fast">Fast</button>
                </div>
              </div>

              <div class="knob-grid">
                ${joints
                  .map(
                    ([jointKey, joint]) => `
                      <article class="knob-card">
                        <button type="button" class="rotary-knob" data-knob="${jointKey}" aria-label="${joint.label} angle control">
                          <div class="knob-indicator" data-thumb="${jointKey}"></div>
                          <div class="knob-center">
                            <div class="knob-value" data-output="${jointKey}">${joint.angle}°</div>
                            <div class="knob-range"><span data-min-label="${jointKey}">${joint.min}°</span> / <span data-max-label="${jointKey}">${joint.max}°</span></div>
                          </div>
                        </button>
                        <div class="knob-label">${joint.label}</div>
                      </article>
                    `
                  )
                  .join("")}
              </div>

              <div class="action-section">
                <div class="action-section-title">Primary Actions</div>
                <div class="action-cluster">
                  <button type="button" class="action-button button-amber" id="goHomeButton">Go Home</button>
                  <button type="button" class="action-button button-blue" id="setStartupPoseButton">Set Startup Pose</button>
                  <button type="button" class="primary-button" id="saveSettingsButton">Save Limits And Pose</button>
                </div>
              </div>

              <div class="action-section">
                <div class="action-section-title">Sequence Control</div>
                <div class="action-cluster">
                  <button type="button" class="action-button button-red${isRecording ? " is-active" : ""}" id="recordButton">${isRecording ? "Stop Record" : "Record"}</button>
                  <button type="button" class="action-button button-green${isPlayingSequence ? " is-active" : ""}" id="playSequenceButton">${isPlayingSequence ? "Stop Play" : "Play"}</button>
                  <button type="button" class="action-button button-violet${loopPlayback ? " is-active" : ""}" id="loopSequenceButton">Loop ${loopPlayback ? "On" : "Off"}</button>
                  <button type="button" class="action-button button-amber" id="clearSequenceButton">Clear</button>
                </div>
                <div class="record-stat" id="sequenceStatus">${sequenceSummaryText()}</div>
              </div>

              <details class="limits-panel" id="limitsPanel"${limitsExpanded ? " open" : ""}>
                <summary class="limits-summary">Advanced Limits</summary>
                <div class="limits-content">
                  <div class="limits-grid">
                    ${joints
                      .map(
                        ([jointKey, joint]) => `
                          <div class="limit-item">
                            <div class="limit-item-header">
                              <div class="limit-item-title">${joint.label}</div>
                              <div class="live-row-meta">Home ${joint.startupAngle}°</div>
                            </div>
                            <div class="limit-pair">
                              <label class="field-stack">
                                <span class="field-label">Min</span>
                                <input type="number" min="0" max="180" value="${joint.min}" data-min="${jointKey}" />
                              </label>
                              <label class="field-stack">
                                <span class="field-label">Max</span>
                                <input type="number" min="0" max="180" value="${joint.max}" data-max="${jointKey}" />
                              </label>
                            </div>
                          </div>
                        `
                      )
                      .join("")}
                  </div>
                </div>
              </details>

              <div class="mini-note">Use a separate 5V servo supply on the expansion board, with ESP32 and servo grounds connected together.</div>
              <div class="record-stat status-note" id="statusNote">${statusMessage}</div>
            </div>
          </div>
        `;

        statusNote = document.getElementById("statusNote");
        applyStatusNoteToDom();

        const limitsPanel = document.getElementById("limitsPanel");
        if (limitsPanel) {
          limitsPanel.addEventListener("toggle", () => {
            limitsExpanded = limitsPanel.open;
          });
        }

        document.querySelectorAll("[data-knob]").forEach((surface) => {
          bindKnobInteraction(surface, surface.dataset.knob);
        });

        document.querySelectorAll("[data-min]").forEach((input) => {
          input.addEventListener("change", (event) => {
            updateDraftLimit(input.dataset.min, "min", Number.parseInt(event.target.value, 10));
          });
        });

        document.querySelectorAll("[data-max]").forEach((input) => {
          input.addEventListener("change", (event) => {
            updateDraftLimit(input.dataset.max, "max", Number.parseInt(event.target.value, 10));
          });
        });

        document.querySelectorAll("[data-speed]").forEach((button) => {
          button.addEventListener("click", () => {
            setSpeedMode(button.dataset.speed);
          });
        });

        document.getElementById("goHomeButton").addEventListener("click", () => {
          postSimple("/api/go-home", "Moved to saved startup pose.");
        });

        document.getElementById("setStartupPoseButton").addEventListener("click", () => {
          postAndMarkSaved("/api/startup-pose", "Startup pose saved on ESP32");
        });

        document.getElementById("saveSettingsButton").addEventListener("click", () => {
          saveSettingsToEsp32();
        });

        document.getElementById("recordButton").addEventListener("click", () => {
          toggleRecording();
        });

        document.getElementById("playSequenceButton").addEventListener("click", () => {
          if (isPlayingSequence) {
            stopPlayback("Playback stopped");
            return;
          }

          playRecordedSequence();
        });

        document.getElementById("loopSequenceButton").addEventListener("click", () => {
          loopPlayback = !loopPlayback;
          saveSequenceState();
          syncLiveControlCard();
          setStatusNote(loopPlayback ? "Loop enabled for recorded move" : "Loop disabled", "is-saved");
        });

        document.getElementById("clearSequenceButton").addEventListener("click", () => {
          clearRecordedSequence();
        });

        syncLiveControlCard();
      }

      function syncLiveControlCard() {
        if (!liveControlCard.children.length) {
          return;
        }

        const badge = document.getElementById("controllerBadge");
        const sequenceStatus = document.getElementById("sequenceStatus");
        const recordButton = document.getElementById("recordButton");
        const playButton = document.getElementById("playSequenceButton");
        const loopButton = document.getElementById("loopSequenceButton");

        if (badge) {
          badge.textContent = controllerBadgeText(liveStatus);
        }

        if (sequenceStatus) {
          sequenceStatus.textContent = sequenceSummaryText();
        }

        if (recordButton) {
          recordButton.textContent = isRecording ? "Stop Record" : "Record";
          recordButton.classList.toggle("is-active", isRecording);
        }

        if (playButton) {
          playButton.textContent = isPlayingSequence ? "Stop Play" : "Play";
          playButton.classList.toggle("is-active", isPlayingSequence);
        }

        if (loopButton) {
          loopButton.textContent = `Loop ${loopPlayback ? "On" : "Off"}`;
          loopButton.classList.toggle("is-active", loopPlayback);
        }

        document.querySelectorAll("[data-speed]").forEach((button) => {
          button.classList.toggle("is-active", button.dataset.speed === selectedSpeedMode());
        });

        Object.entries(draftSettings).forEach(([jointKey, joint]) => {
          const surface = document.querySelector(`[data-knob="${jointKey}"]`);
          const thumb = document.querySelector(`[data-thumb="${jointKey}"]`);
          const output = document.querySelector(`[data-output="${jointKey}"]`);
          const minLabel = document.querySelector(`[data-min-label="${jointKey}"]`);
          const maxLabel = document.querySelector(`[data-max-label="${jointKey}"]`);
          const minInput = document.querySelector(`[data-min="${jointKey}"]`);
          const maxInput = document.querySelector(`[data-max="${jointKey}"]`);

          if (surface && thumb) {
            surface.style.setProperty("--angle", `${angleToKnobRotation(joint)}deg`);
            surface.style.setProperty("--progress", angleToKnobProgress(joint));
          }

          if (output) {
            output.textContent = `${joint.angle}°`;
          }

          if (minLabel) {
            minLabel.textContent = `${joint.min}°`;
          }

          if (maxLabel) {
            maxLabel.textContent = `${joint.max}°`;
          }

          if (minInput && document.activeElement !== minInput) {
            minInput.value = String(joint.min);
          }

          if (maxInput && document.activeElement !== maxInput) {
            maxInput.value = String(joint.max);
          }
        });

        updateRobotVisualization();
        applyStatusNoteToDom();
      }

      function recordCurrentPose(force = false) {
        if (!isRecording || !Object.keys(draftSettings).length) {
          return;
        }

        const elapsed = Math.max(0, Math.round(performance.now() - recordStartedAt));
        const pose = getPoseSnapshot();
        const lastFrame = recordedFrames[recordedFrames.length - 1];

        if (lastFrame && samePose(lastFrame.pose, pose)) {
          if (force) {
            lastFrame.at = elapsed;
            saveSequenceState();
            syncLiveControlCard();
          }
          return;
        }

        if (lastFrame && !force && elapsed - lastRecordedAt < RECORD_FRAME_MIN_MS) {
          lastFrame.at = elapsed;
          lastFrame.pose = pose;
        } else {
          recordedFrames.push({ at: elapsed, pose });
        }

        lastRecordedAt = elapsed;
        saveSequenceState();
        syncLiveControlCard();
      }

      function stopPlayback(note = "") {
        const wasPlaying = isPlayingSequence;
        playbackRunId += 1;

        if (playbackTimer) {
          clearTimeout(playbackTimer);
          playbackTimer = null;
        }

        isPlayingSequence = false;
        syncLiveControlCard();
        updateNetworkCards(liveStatus);

        if (note && wasPlaying) {
          setStatusNote(note, "is-dirty");
        }
      }

      function clearRecordedSequence() {
        isRecording = false;
        stopPlayback("");
        recordedFrames = [];
        saveSequenceState();
        syncLiveControlCard();
        markSaved("Recorded move cleared");
      }

      function toggleRecording() {
        if (isRecording) {
          recordCurrentPose(true);
          isRecording = false;
          saveSequenceState();
          syncLiveControlCard();
          updateNetworkCards(liveStatus);
          markSaved(`Recording saved (${recordedFrames.length} frames)`);
          return;
        }

        stopPlayback("");
        isRecording = true;
        recordStartedAt = performance.now();
        lastRecordedAt = 0;
        recordedFrames = [{ at: 0, pose: getPoseSnapshot() }];
        saveSequenceState();
        syncLiveControlCard();
        updateNetworkCards(liveStatus);
        setStatusNote("Recording live move sequence", "is-dirty");
      }

      function updateDraftAngle(jointKey, nextAngle, sendMove, source = "manual") {
        const joint = draftSettings[jointKey];
        if (!joint) {
          return;
        }

        if (source === "manual" && isPlayingSequence) {
          stopPlayback("Playback stopped for manual control");
        }

        const safeAngle = Number.isInteger(nextAngle) ? clamp(nextAngle, joint.min, joint.max) : joint.angle;
        joint.angle = safeAngle;
        syncLiveControlCard();
        markDirty("Pose changed. Save if this should be kept on the ESP32.");

        if (source === "manual") {
          recordCurrentPose();
        }

        if (sendMove) {
          queueMove(jointKey, safeAngle);
        }
      }

      function updateDraftLimit(jointKey, limitKey, rawValue) {
        const joint = draftSettings[jointKey];
        if (!joint) {
          return;
        }

        if (isPlayingSequence) {
          stopPlayback("Playback stopped for manual control");
        }

        const safeValue = Number.isInteger(rawValue) ? clamp(rawValue, 0, 180) : joint[limitKey];
        joint[limitKey] = safeValue;

        if (joint.min > joint.max) {
          if (limitKey === "min") {
            joint.max = joint.min;
          } else {
            joint.min = joint.max;
          }
        }

        joint.angle = clamp(joint.angle, joint.min, joint.max);
        syncLiveControlCard();
        markDirty("Limits changed. Save to keep them on the ESP32.");
        queueMove(jointKey, joint.angle);
      }

      function queueMove(jointKey, angle) {
        moveQueuedAngles[jointKey] = angle;

        if (moveTimers[jointKey] || moveInFlight[jointKey]) {
          return;
        }

        const now = Date.now();
        const elapsed = now - (lastMoveSentAt[jointKey] || 0);
        const waitMs = Math.max(0, 10 - elapsed);
        moveTimers[jointKey] = window.setTimeout(() => {
          sendQueuedMove(jointKey);
        }, waitMs);
      }

      async function sendQueuedMove(jointKey) {
        const angle = moveQueuedAngles[jointKey];
        moveQueuedAngles[jointKey] = undefined;
        moveTimers[jointKey] = null;
        moveInFlight[jointKey] = true;
        lastMoveSentAt[jointKey] = Date.now();

        try {
          liveStatus = await api("/api/move", {
            method: "POST",
            headers: {
              "Content-Type": "application/x-www-form-urlencoded",
            },
            body: toForm({ joint: jointKey, angle }),
          });
          updateNetworkCards(liveStatus);
          syncLiveControlCard();
        } catch (error) {
          setStatusNote(error.message, "is-dirty");
        } finally {
          moveInFlight[jointKey] = false;
          if (moveQueuedAngles[jointKey] !== undefined) {
            queueMove(jointKey, moveQueuedAngles[jointKey]);
          }
        }
      }

      async function setSpeedMode(speedMode) {
        try {
          liveStatus = await api("/api/speed", {
            method: "POST",
            headers: {
              "Content-Type": "application/x-www-form-urlencoded",
            },
            body: toForm({ speed: speedMode }),
          });
          syncLiveControlCard();
          setStatusNote(`Speed set to ${speedMode}`, "is-saved");
        } catch (error) {
          setStatusNote(error.message, "is-dirty");
        }
      }

      function waitForPlayback(milliseconds, runId) {
        return new Promise((resolve) => {
          playbackTimer = window.setTimeout(() => {
            playbackTimer = null;
            resolve(runId === playbackRunId);
          }, Math.max(70, milliseconds));
        });
      }

      async function sendPoseToEsp32(pose) {
        const formData = {};

        Object.entries(pose).forEach(([jointKey, angle]) => {
          formData[`${jointKey}_angle`] = angle;
        });

        liveStatus = await api("/api/pose", {
          method: "POST",
          headers: {
            "Content-Type": "application/x-www-form-urlencoded",
          },
          body: toForm(formData),
        });

        copyDraftFromStatus(liveStatus);
        syncLiveControlCard();
        updateNetworkCards(liveStatus);
      }

      async function playRecordedSequence() {
        if (!recordedFrames.length) {
          setStatusNote("Record a move first", "is-dirty");
          return;
        }

        if (isRecording) {
          toggleRecording();
        }

        stopPlayback("");
        const runId = ++playbackRunId;
        isPlayingSequence = true;
        syncLiveControlCard();
        updateNetworkCards(liveStatus);
        setStatusNote(loopPlayback ? "Playing recorded move in loop" : "Playing recorded move", "is-dirty");

        try {
          do {
            for (let index = 0; index < recordedFrames.length; index += 1) {
              if (runId !== playbackRunId) {
                return;
              }

              await sendPoseToEsp32(getPoseFromFrame(recordedFrames[index]));

              const nextFrame = recordedFrames[index + 1];
              if (!nextFrame) {
                continue;
              }

              const keepGoing = await waitForPlayback(nextFrame.at - recordedFrames[index].at, runId);
              if (!keepGoing) {
                return;
              }
            }

            if (!loopPlayback) {
              break;
            }

            const continueLoop = await waitForPlayback(220, runId);
            if (!continueLoop) {
              return;
            }
          } while (loopPlayback && runId === playbackRunId);
        } catch (error) {
          setStatusNote(error.message, "is-dirty");
        } finally {
          if (runId === playbackRunId) {
            isPlayingSequence = false;
            syncLiveControlCard();
            updateNetworkCards(liveStatus);
            if (!loopPlayback) {
              markSaved("Playback finished");
            }
          }
        }
      }

      async function refreshStatus(forceSyncDraft = false) {
        try {
          const status = await api("/api/status");
          liveStatus = status;
          updateNetworkCards(status);

          if (!draftDirty || forceSyncDraft || !Object.keys(draftSettings).length) {
            copyDraftFromStatus(status);
            if (!liveControlCard.children.length || forceSyncDraft || !Object.keys(draftSettings).length) {
              renderLiveControlCard();
            } else {
              syncLiveControlCard();
            }
            if (!draftDirty) {
              markSaved("ESP32 online");
            }
          } else {
            syncLiveControlCard();
          }
        } catch (error) {
          document.getElementById("deviceStatus").textContent = "Offline";
          document.getElementById("deviceStatus").classList.add("offline");
          document.getElementById("deviceMeta").textContent = error.message;
          setStatusNote("ESP32 is restarting or unreachable", "is-dirty");
        }
      }

      async function postSimple(path, noteOnSuccess) {
        stopPlayback("");

        try {
          liveStatus = await api(path, { method: "POST" });
          copyDraftFromStatus(liveStatus);
          renderLiveControlCard();
          updateNetworkCards(liveStatus);
          markDirty(noteOnSuccess);
        } catch (error) {
          setStatusNote(error.message, "is-dirty");
        }
      }

      async function postAndMarkSaved(path, noteOnSuccess) {
        stopPlayback("");

        try {
          liveStatus = await api(path, { method: "POST" });
          copyDraftFromStatus(liveStatus);
          renderLiveControlCard();
          updateNetworkCards(liveStatus);
          markSaved(noteOnSuccess);
        } catch (error) {
          setStatusNote(error.message, "is-dirty");
        }
      }

      async function saveSettingsToEsp32() {
        const formData = {};
        stopPlayback("");

        Object.entries(draftSettings).forEach(([jointKey, joint]) => {
          formData[`${jointKey}_min`] = joint.min;
          formData[`${jointKey}_max`] = joint.max;
          formData[`${jointKey}_angle`] = joint.angle;
        });

        try {
          liveStatus = await api("/api/settings", {
            method: "POST",
            headers: {
              "Content-Type": "application/x-www-form-urlencoded",
            },
            body: toForm(formData),
          });
          copyDraftFromStatus(liveStatus);
          renderLiveControlCard();
          updateNetworkCards(liveStatus);
          markSaved("Limits and pose saved on ESP32");
        } catch (error) {
          setStatusNote(error.message, "is-dirty");
        }
      }

      window.addEventListener("load", async () => {
        loadSequenceState();
        await refreshStatus(true);
        refreshInterval = window.setInterval(() => {
          refreshStatus(false);
        }, 1800);
      });

      window.addEventListener("mousemove", moveActiveFader);
      window.addEventListener("mouseup", endActiveFader);
      window.addEventListener("touchmove", moveActiveFader, { passive: false });
      window.addEventListener("touchend", endActiveFader, { passive: true });
      window.addEventListener("touchcancel", endActiveFader, { passive: true });
    </script>
  </body>
</html>
)HTML";

WebServer server(80);
DNSServer dnsServer;
Preferences preferences;
JointConfig jointConfigs[SERVO_COUNT];
int startupAngles[SERVO_COUNT];

bool danceActive = false;
size_t danceStepIndex = 0;
unsigned long danceStepStartedAt = 0;
bool startupPosePending = true;
unsigned long startupPoseAt = 0;
bool pcaReady = false;
enum SpeedMode : uint8_t {
  SPEED_SLOW,
  SPEED_MEDIUM,
  SPEED_FAST
};
SpeedMode speedMode = SPEED_SLOW;

const char* speedModeText() {
  if (speedMode == SPEED_FAST) {
    return "fast";
  }
  if (speedMode == SPEED_MEDIUM) {
    return "medium";
  }
  return "slow";
}

int clampValue(int value, int minValue, int maxValue) {
  if (value < minValue) {
    return minValue;
  }

  if (value > maxValue) {
    return maxValue;
  }

  return value;
}

uint32_t microsecondsToDuty(uint16_t microseconds) {
  const uint32_t maxDuty = (1UL << PWM_RESOLUTION_BITS) - 1UL;
  const uint32_t periodUs = 1000000UL / PWM_FREQUENCY;
  return ((uint32_t)microseconds * maxDuty) / periodUs;
}

bool initServoPwm() {
  bool ok = true;
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    ok = ledcAttach(SERVO_PINS[jointIndex], PWM_FREQUENCY, PWM_RESOLUTION_BITS) && ok;
  }
  return ok;
}

bool writeServoPulse(uint8_t jointIndex, int angle) {
  if (jointIndex >= SERVO_COUNT) {
    return false;
  }

  const uint16_t pulseUs = map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  return ledcWrite(SERVO_PINS[jointIndex], microsecondsToDuty(pulseUs));
}

void normalizeJointConfig(uint8_t jointIndex) {
  jointConfigs[jointIndex].minAngle = clampValue(jointConfigs[jointIndex].minAngle, 0, 180);
  jointConfigs[jointIndex].maxAngle = clampValue(jointConfigs[jointIndex].maxAngle, 0, 180);

  if (jointConfigs[jointIndex].minAngle > jointConfigs[jointIndex].maxAngle) {
    const int temp = jointConfigs[jointIndex].minAngle;
    jointConfigs[jointIndex].minAngle = jointConfigs[jointIndex].maxAngle;
    jointConfigs[jointIndex].maxAngle = temp;
  }

  jointConfigs[jointIndex].angle =
    clampValue(jointConfigs[jointIndex].angle, jointConfigs[jointIndex].minAngle, jointConfigs[jointIndex].maxAngle);
}

void setServoAngle(uint8_t jointIndex, int angle) {
  angle = clampValue(angle, jointConfigs[jointIndex].minAngle, jointConfigs[jointIndex].maxAngle);
  if (pcaReady) {
    int currentAngle = jointConfigs[jointIndex].angle;
    currentAngle = clampValue(currentAngle, jointConfigs[jointIndex].minAngle, jointConfigs[jointIndex].maxAngle);
    uint8_t rampStep = MG90S_RAMP_STEP_DEG;
    uint8_t rampDelayMs = MG90S_RAMP_DELAY_MS;

    if (speedMode == SPEED_FAST) {
      rampStep = 180;
      rampDelayMs = 0;
    } else if (speedMode == SPEED_MEDIUM) {
      rampStep = MG90S_MEDIUM_RAMP_STEP_DEG;
      rampDelayMs = MG90S_MEDIUM_RAMP_DELAY_MS;
    }

    if (currentAngle == angle || speedMode == SPEED_FAST) {
      writeServoPulse(jointIndex, angle);
    } else {
      const int direction = angle > currentAngle ? rampStep : -rampStep;
      for (int nextAngle = currentAngle; nextAngle != angle; nextAngle += direction) {
        writeServoPulse(jointIndex, nextAngle);
        delay(rampDelayMs);

        if ((direction > 0 && nextAngle + direction > angle) || (direction < 0 && nextAngle + direction < angle)) {
          break;
        }
      }

      writeServoPulse(jointIndex, angle);
    }
  }
  jointConfigs[jointIndex].angle = angle;
}

void applyCurrentPose() {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    setServoAngle(jointIndex, jointConfigs[jointIndex].angle);
    delay(120);
  }
}

void applyStartupPose() {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    setServoAngle(jointIndex, startupAngles[jointIndex]);
    delay(120);
  }
}

void centerAllServos() {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    setServoAngle(jointIndex, SERVO_CENTER_DEG);
    delay(120);
  }
}

void stopDance() {
  danceActive = false;
  danceStepIndex = 0;
}

void applyDanceStep(size_t stepIndex) {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    setServoAngle(jointIndex, DANCE_STEPS[stepIndex].angles[jointIndex]);
  }

  danceStepStartedAt = millis();
}

void startDance() {
  danceActive = true;
  danceStepIndex = 0;
  applyDanceStep(danceStepIndex);
}

void updateDance() {
  if (!danceActive) {
    return;
  }

  if (millis() - danceStepStartedAt < DANCE_STEPS[danceStepIndex].holdMs) {
    return;
  }

  danceStepIndex++;
  if (danceStepIndex >= (sizeof(DANCE_STEPS) / sizeof(DANCE_STEPS[0]))) {
    stopDance();
    return;
  }

  applyDanceStep(danceStepIndex);
}

String jsonEscape(const String &input) {
  String escaped = input;
  escaped.replace("\\", "\\\\");
  escaped.replace("\"", "\\\"");
  escaped.replace("\n", "\\n");
  escaped.replace("\r", "");
  return escaped;
}

int getJointIndex(const String &jointName) {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    if (jointName.equalsIgnoreCase(JOINT_NAMES[jointIndex])) {
      return jointIndex;
    }
  }

  return -1;
}

int getArgAsInt(const String &name, int fallbackValue) {
  if (!server.hasArg(name)) {
    return fallbackValue;
  }

  return server.arg(name).toInt();
}

void loadPreferences() {
  preferences.begin(PREFERENCES_NAMESPACE, false);

  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    const int savedAngle = preferences.getInt(ANGLE_PREF_KEYS[jointIndex], SERVO_CENTER_DEG);
    jointConfigs[jointIndex].angle = savedAngle;
    jointConfigs[jointIndex].minAngle = preferences.getInt(MIN_PREF_KEYS[jointIndex], 0);
    jointConfigs[jointIndex].maxAngle = preferences.getInt(MAX_PREF_KEYS[jointIndex], 180);
    normalizeJointConfig(jointIndex);
    startupAngles[jointIndex] = preferences.getInt(STARTUP_PREF_KEYS[jointIndex], savedAngle);
    startupAngles[jointIndex] =
      clampValue(startupAngles[jointIndex], jointConfigs[jointIndex].minAngle, jointConfigs[jointIndex].maxAngle);
  }
}

void saveJointSettings() {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    preferences.putInt(ANGLE_PREF_KEYS[jointIndex], jointConfigs[jointIndex].angle);
    preferences.putInt(MIN_PREF_KEYS[jointIndex], jointConfigs[jointIndex].minAngle);
    preferences.putInt(MAX_PREF_KEYS[jointIndex], jointConfigs[jointIndex].maxAngle);
  }
}

void saveStartupPose() {
  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    startupAngles[jointIndex] =
      clampValue(startupAngles[jointIndex], jointConfigs[jointIndex].minAngle, jointConfigs[jointIndex].maxAngle);
    preferences.putInt(STARTUP_PREF_KEYS[jointIndex], startupAngles[jointIndex]);
  }
}

String buildStatusJson() {
  String json = "{";
  json += "\"ok\":true,";
  json += "\"apSsid\":\"" + jsonEscape(String(AP_SSID)) + "\",";
  json += "\"apIp\":\"" + WiFi.softAPIP().toString() + "\",";
  json += "\"clientIp\":\"" + server.client().remoteIP().toString() + "\",";
  json += "\"pcaReady\":";
  json += pcaReady ? "true" : "false";
  json += ",";
  json += "\"danceActive\":";
  json += danceActive ? "true" : "false";
  json += ",";
  json += "\"speedMode\":\"" + String(speedModeText()) + "\",";
  json += "\"joints\":[";

  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    if (jointIndex > 0) {
      json += ",";
    }

    json += "{";
    json += "\"key\":\"" + String(JOINT_NAMES[jointIndex]) + "\",";
    json += "\"label\":\"" + String(JOINT_LABELS[jointIndex]) + "\",";
    json += "\"channel\":" + String(jointIndex) + ",";
    json += "\"angle\":" + String(jointConfigs[jointIndex].angle) + ",";
    json += "\"min\":" + String(jointConfigs[jointIndex].minAngle) + ",";
    json += "\"max\":" + String(jointConfigs[jointIndex].maxAngle) + ",";
    json += "\"startupAngle\":" + String(startupAngles[jointIndex]);
    json += "}";
  }

  json += "]";
  json += "}";
  return json;
}

void sendStatusJson() {
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", buildStatusJson());
}

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleStatus() {
  sendStatusJson();
}

void handlePing() {
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "text/plain", "ok");
}

void handleMove() {
  if (!server.hasArg("joint") || !server.hasArg("angle")) {
    server.send(400, "text/plain", "Missing joint or angle");
    return;
  }

  const int jointIndex = getJointIndex(server.arg("joint"));
  if (jointIndex < 0) {
    server.send(400, "text/plain", "Invalid joint");
    return;
  }

  stopDance();
  setServoAngle((uint8_t)jointIndex, clampValue(server.arg("angle").toInt(), 0, 180));
  sendStatusJson();
}

void handleSpeed() {
  const String speed = server.arg("speed");
  if (speed.equalsIgnoreCase("fast")) {
    speedMode = SPEED_FAST;
  } else if (speed.equalsIgnoreCase("medium")) {
    speedMode = SPEED_MEDIUM;
  } else {
    speedMode = SPEED_SLOW;
  }

  sendStatusJson();
}

void handlePose() {
  bool hasPoseValue = false;
  stopDance();

  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    const String argName = String(JOINT_NAMES[jointIndex]) + "_angle";
    if (!server.hasArg(argName)) {
      continue;
    }

    hasPoseValue = true;
    setServoAngle(jointIndex, clampValue(server.arg(argName).toInt(), 0, 180));
  }

  if (!hasPoseValue) {
    server.send(400, "text/plain", "Missing pose values");
    return;
  }

  sendStatusJson();
}

void handleCenter() {
  stopDance();
  centerAllServos();
  sendStatusJson();
}

void handleGoHome() {
  stopDance();
  applyStartupPose();
  sendStatusJson();
}

void handleSaveSettings() {
  stopDance();

  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    const String jointName = JOINT_NAMES[jointIndex];
    jointConfigs[jointIndex].minAngle =
      clampValue(getArgAsInt(jointName + "_min", jointConfigs[jointIndex].minAngle), 0, 180);
    jointConfigs[jointIndex].maxAngle =
      clampValue(getArgAsInt(jointName + "_max", jointConfigs[jointIndex].maxAngle), 0, 180);

    if (jointConfigs[jointIndex].minAngle > jointConfigs[jointIndex].maxAngle) {
      const int temp = jointConfigs[jointIndex].minAngle;
      jointConfigs[jointIndex].minAngle = jointConfigs[jointIndex].maxAngle;
      jointConfigs[jointIndex].maxAngle = temp;
    }

    jointConfigs[jointIndex].angle =
      getArgAsInt(jointName + "_angle", jointConfigs[jointIndex].angle);
    normalizeJointConfig(jointIndex);
    startupAngles[jointIndex] =
      clampValue(startupAngles[jointIndex], jointConfigs[jointIndex].minAngle, jointConfigs[jointIndex].maxAngle);
    setServoAngle(jointIndex, jointConfigs[jointIndex].angle);
  }

  saveJointSettings();
  saveStartupPose();
  sendStatusJson();
}

void handleSetStartupPose() {
  stopDance();

  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    startupAngles[jointIndex] = jointConfigs[jointIndex].angle;
  }

  saveStartupPose();
  sendStatusJson();
}

void handleDefaults() {
  stopDance();

  for (uint8_t jointIndex = 0; jointIndex < SERVO_COUNT; jointIndex++) {
    jointConfigs[jointIndex].minAngle = 0;
    jointConfigs[jointIndex].maxAngle = 180;
    jointConfigs[jointIndex].angle = SERVO_CENTER_DEG;
    startupAngles[jointIndex] = SERVO_CENTER_DEG;
  }

  centerAllServos();
  saveJointSettings();
  saveStartupPose();
  sendStatusJson();
}

void handleDance() {
  const String action = server.arg("action");

  if (action.equalsIgnoreCase("stop")) {
    stopDance();
  } else {
    startDance();
  }

  sendStatusJson();
}

void handleRedirectToRoot() {
  server.sendHeader("Location", String("http://") + AP_IP.toString() + "/", true);
  server.send(302, "text/plain", "");
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/ping", HTTP_GET, handlePing);
  server.on("/generate_204", HTTP_GET, handleRedirectToRoot);
  server.on("/gen_204", HTTP_GET, handleRedirectToRoot);
  server.on("/hotspot-detect.html", HTTP_GET, handleRedirectToRoot);
  server.on("/library/test/success.html", HTTP_GET, handleRedirectToRoot);
  server.on("/connecttest.txt", HTTP_GET, handleRedirectToRoot);
  server.on("/connectivity-check.html", HTTP_GET, handleRedirectToRoot);
  server.on("/success.txt", HTTP_GET, handleRedirectToRoot);
  server.on("/canonical.html", HTTP_GET, handleRedirectToRoot);
  server.on("/ncsi.txt", HTTP_GET, handleRedirectToRoot);
  server.on("/fwlink", HTTP_GET, handleRedirectToRoot);
  server.on("/redirect", HTTP_GET, handleRedirectToRoot);
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/move", HTTP_POST, handleMove);
  server.on("/api/speed", HTTP_POST, handleSpeed);
  server.on("/api/pose", HTTP_POST, handlePose);
  server.on("/api/go-home", HTTP_POST, handleGoHome);
  server.on("/api/center", HTTP_POST, handleCenter);
  server.on("/api/settings", HTTP_POST, handleSaveSettings);
  server.on("/api/startup-pose", HTTP_POST, handleSetStartupPose);
  server.on("/api/defaults", HTTP_POST, handleDefaults);
  server.on("/api/dance", HTTP_POST, handleDance);
  server.onNotFound(handleRedirectToRoot);
  server.begin();
}

void startNetworking() {
  WiFi.persistent(false);
  WiFi.setSleep(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP SSID: ");
  Serial.println(AP_SSID);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  dnsServer.start(DNS_PORT, "*", AP_IP);
  Serial.println("Captive portal DNS started.");
}

void setup() {
  Serial.begin(115200);
  delay(400);
  Serial.println();
  Serial.println("Robot Arm Pro Wi-Fi Controller");

  loadPreferences();

  if (!initServoPwm()) {
    pcaReady = false;
    Serial.println("Servo PWM failed. Starting Wi-Fi without servo output.");
  } else {
    pcaReady = true;
    Serial.println("Direct ESP32 servo PWM ready.");
    Serial.println("P18 clamp, P4 shoulder, P32 elbow, P13 base.");
  }

  startNetworking();
  setupWebServer();
  startupPoseAt = millis() + STARTUP_MOVE_DELAY_MS;

  Serial.println("HTTP server ready.");
  Serial.println("Hotspot starts before arm motion.");
  Serial.println("Open the captive portal popup or browse to http://192.168.4.1/.");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  updateDance();

  if (startupPosePending && millis() >= startupPoseAt) {
    applyStartupPose();
    startupPosePending = false;
  }
}
