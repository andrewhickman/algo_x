import sys

import pygame


class TileRenderer:
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((800, 800))
        self.colours = {
            'F': (131, 148, 150),
            'L': (147, 161, 161),
            'N': (238, 232, 213),
            'P': (253, 246, 227),
            'Y': (181, 137,   0),
            'T': (203,  75,  22),
            'U': (220,  50,  47),
            'V': (211,  54, 130),
            'W': (108, 113, 196),
            'Z': ( 38, 139, 210),
            'I': ( 42, 161, 152),
            'X': (133, 153,   0),
            ' ': (  0,   0,   0)
        }
            

    def render(self, tiling):
        for x in range(8):
            for y in range(8):
                char = tiling[x + 8 * y]
                self.screen.fill(self.colours[char], (100 * x, 100 * y, 100, 100))
        pygame.display.flip()

if __name__ == '__main__':
    r = TileRenderer()
    for num, tiling in enumerate(sys.stdin):
        r.render(tiling)
        pygame.image.save(r.screen, "images/tiling_" + str(num) + ".png")
