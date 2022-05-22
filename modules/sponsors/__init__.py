import tidal
from textwindow import TextWindow
from app import PagedApp
from buttons import Buttons
import lodepng
from . import sponsor1_png
from . import sponsor2_png
from . import sponsor3_png

class ImageWindow(TextWindow):
    """Simple window class that just displays a single image, centred on screen"""
    def __init__(self, frozen_img, buttons):
        super().__init__(tidal.BLACK, tidal.BLUE, None, None, buttons)
        self.display = tidal.display
        self.img = frozen_img

    def redraw(self):
        self.cls()
        (w, h, buf) = lodepng.decode565(self.img.DATA)
        x = (self.width() - w) // 2
        y = (self.height() - h) // 2
        self.display.blit_buffer(buf, 0, 0, w, h)

class Sponsors(PagedApp):

    def __init__(self):
        super().__init__()
        # Normally switching pages would cancel any autorepeat on the left/right
        # buttons, sharing a common Buttons instance avoids that.
        shared_buttons = Buttons()
        self.pages = (
            ImageWindow(sponsor1_png, shared_buttons),
            ImageWindow(sponsor2_png, shared_buttons),
            ImageWindow(sponsor3_png, shared_buttons),
        )

    def on_activate(self):
        super().on_activate()
