"""SVG -> PNG in Kacheln (QSvgRenderer). Aufruf: render_svg.py <svg> <outdir> <cols> <rows> [breite_px]"""
import sys
from pathlib import Path

from PySide6.QtCore import QRectF
from PySide6.QtGui import QColor, QGuiApplication, QImage, QPainter
from PySide6.QtSvg import QSvgRenderer

svg, out, cols, rows = Path(sys.argv[1]), Path(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
total_w = int(sys.argv[5]) if len(sys.argv) > 5 else 6000
app = QGuiApplication([])
r = QSvgRenderer(str(svg))
vb = r.viewBoxF()
scale = total_w / vb.width()
total_h = int(vb.height() * scale)
img = QImage(total_w, total_h, QImage.Format.Format_RGB32)
img.fill(QColor("white"))
p = QPainter(img)
p.setRenderHint(QPainter.RenderHint.Antialiasing)
r.render(p, QRectF(0, 0, total_w, total_h))
p.end()
out.mkdir(parents=True, exist_ok=True)
img.scaled(1800, int(1800 * total_h / total_w)).save(str(out / "overview.png"))
tw, th = total_w // cols, total_h // rows
ov = 80
for y in range(rows):
    for x in range(cols):
        tile = img.copy(max(0, x * tw - ov), max(0, y * th - ov), tw + 2 * ov, th + 2 * ov)
        tile.save(str(out / f"tile_{y}_{x}.png"))
print(total_w, total_h)
