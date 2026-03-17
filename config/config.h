#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  Central configuration — every tunable constant in one place.
//  Change a value here and rebuild; the whole app picks it up.
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include <QColor>
#include <QString>

namespace config {

// ── Shapes: defaults ──────────────────────────────────────────
inline constexpr double kDefaultBboxX = 100.0;
inline constexpr double kDefaultBboxY = 100.0;
inline constexpr double kDefaultBboxW = 100.0;
inline constexpr double kDefaultBboxH = 100.0;

inline const QColor kDefaultFillColour = QColor(30, 144, 255); 
inline const QColor kDefaultStrokeColour = QColor(0, 0, 0);     // Black
inline constexpr int kDefaultStrokeWidth = 1;

// ── Shapes: selection handles ─────────────────────────────────
inline constexpr double kHandleSize =
    10.0;  // side length of resize handles (px)
inline constexpr double kMinShapeSize = 8.0;  // minimum bbox dimension (px)
inline constexpr int kHandlePenWidth = 1;     // outline pen width for handles

// ── Line: defaults ────────────────────────────────────────────
inline constexpr double kDefaultLineX1 = 100.0;
inline constexpr double kDefaultLineY1 = 100.0;
inline constexpr double kDefaultLineX2 = 200.0;
inline constexpr double kDefaultLineY2 = 200.0;
inline constexpr double kLineBboxPad =
    1.0;  // px padding each side around endpoints

// ── Polyline: defaults ────────────────────────────────────────
inline constexpr double kDefaultPolylineCx = 150.0;
inline constexpr double kDefaultPolylineCy = 150.0;
inline constexpr double kPolylineScaleEps = 1e-9;  // near-zero guard for scale

// ── TextBox: defaults ─────────────────────────────────────────
inline const QString kDefaultText = "Input Text";
inline const QString kDefaultFontFamily = "Arial";
inline constexpr int kDefaultFontSize = 15;
inline constexpr double kTextPadLeft = 4.0;   // px left inset for text
inline constexpr double kTextPadTotal = 8.0;  // px total horizontal inset

// ── RoundedRectangle ──────────────────────────────────────────
inline constexpr double kMaxCornerRadiusRatio =
    0.5;  // max radius = 50% of short side

// ── Freehand drawing ──────────────────────────────────────────
inline constexpr int kFreehandSampleDist =
    4;  // Manhattan px between sampled points
inline constexpr int kFreehandPreviewWidth =
    2;  // stroke width of in-progress preview
inline const QColor kFreehandPreviewColour =
    QColor(0, 0, 0);  // preview stroke colour
inline const QColor kFreehandDefaultStroke =
    QColor(0, 0, 0);  // finished polyline stroke
inline constexpr int kFreehandDefaultWidth =
    2;  // finished polyline stroke width

// ── Clipboard ─────────────────────────────────────────────────
inline constexpr double kPasteOffsetX = 20.0;  // px right offset on paste
inline constexpr double kPasteOffsetY = 20.0;  // px down offset on paste

// ── Window & layout ──────────────────────────────────────────
inline constexpr int kWindowMinWidth = 1000;
inline constexpr int kWindowMinHeight = 800;

// ── UI panel colours (CSS hex strings) ───────────────────────
inline const QString kCanvasBgColour = "#c2c2c2";
inline const QString kColourPanelBgColour = "#191919";
inline const QString kControlPanelBgColour = "#101010";
inline const QString kShapePanelBgColour = "#000000";
inline const QString kSketchCheckedColour = "#555555";

// ── Floating context menu ────────────────────────────────────
inline constexpr int kMenuMargin = 4;        // px margin on each side
inline constexpr int kMenuSpacing = 10;      // px between buttons
inline constexpr int kMenuButtonWidth = 80;  // px width of each button

// ── Colour ribbon panel ──────────────────────────────────────
inline constexpr int kPanelSpacing = 12;  // px between groups
inline constexpr int kStrokeWidthMin = 1;
inline constexpr int kStrokeWidthMax = 20;
inline constexpr int kFontSizeMin = 10;
inline constexpr int kFontSizeMax = 40;

}  // namespace config

#endif  // CONFIG_CONFIG_H_
