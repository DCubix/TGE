#include <string>
#include <fstream>
#include <sstream>

#include "core/tgEngine.h"
#include "core/tgLog.h"
#include "core/tgInput.h"

#include "math/tgMath.h"

#include "assets/tgAssets.h"

#include "tween/tgTweens.h"
#include "tween/tgTimer.h"

#include "ecs/tgECS.h"
#include "components/tgSprite.h"
#include "components/tgText.h"
#include "components/tgTransform.h"

#include "systems/tgSpriteSystem.h"
#include "systems/tgTextSystem.h"

#include "core/tgParticleEngine.h"

#include <vector>
#include <cstdlib>

#define GAME_O 1
#define GAME_X 2
#define GAME_TIE 3
#define GAME_NULL 0

typedef struct Piece : public tgComponent {
} Piece;

typedef struct Move {
	Move() : x(0), y(0) {}
	Move(int score) : score(score), x(0), y(0) {}

	int x, y, score;
} Move;

typedef struct Board {
	Board() {
		clear();
	}

	Board(Board const& b) {
		for (int i = 0; i < 9; i++) {
			cells[i] = b.cells[i];
		}
	}

	int get(int x, int y) {
		return cells[y * 3 + x];
	}

	void set(int x, int y, int val) {
		cells[y * 3 + x] = val;
	}

	int checkVictory() {
		bool win;
		int c;

		// Check rows
		for (int y = 0; y < 3; y++) {
			c = get(0, y);
			if (c != GAME_NULL) {
				win = true;
				for (int x = 0; x < 3; x++) {
					if (get(x, y) != c) {
						win = false;
						break;
					}
				}
				if (win) return c;
			}
		}

		// Check columns
		for (int x = 0; x < 3; x++) {
			c = get(x, 0);
			if (c != GAME_NULL) {
				win = true;
				for (int y = 0; y < 3; y++) {
					if (get(x, y) != c) {
						win = false;
						break;
					}
				}
				if (win) return c;
			}
		}

		// Check tl diag
		c = get(0, 0);
		if (c != GAME_NULL) {
			win = true;
			for (int i = 0; i < 3; i++) {
				if (get(i, i) != c) {
					win = false;
					break;
				}
			}
			if (win) return c;
		}

		// Check tr diag
		c = get(2, 0);
		if (c != GAME_NULL) {
			win = true;
			for (int i = 0; i < 3; i++) {
				if (get(2 - i, i) != c) {
					win = false;
					break;
				}
			}
			if (win) return c;
		}

		// Check tie
		for (int i = 0; i < 9; i++) {
			if (cells[i] == GAME_NULL) return GAME_NULL;
		}

		return GAME_TIE;
	}

	void clear() {
		for (int i = 0; i < 9; i++) {
			cells[i] = GAME_NULL;
		}
	}

	int cells[9];
} Board;

typedef struct AI {
	AI() {}

	Move performMove(Board &board) {
		Move mv = bestMove(board, GAME_X);
		board.set(mv.x, mv.y, GAME_X);
		return mv;
	}

private:
	Move bestMove(Board &board, int player) {
		int res = board.checkVictory();
		if (res == GAME_X) { // AI
			return Move(10);
		} else if (res == GAME_O) { // PLAYER
			return Move(-10);
		} else if (res == GAME_TIE) {
			return Move(0);
		}

		std::vector<Move> moves;

		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				if (board.get(x, y) == GAME_NULL) {
					Move mv;
					mv.x = x;
					mv.y = y;
					board.set(x, y, player);
					if (player == GAME_X) {
						mv.score = bestMove(board, GAME_O).score;
					} else {
						mv.score = bestMove(board, GAME_X).score;
					}
					moves.push_back(mv);
					board.set(x, y, GAME_NULL);
				}
			}
		}

		int best_move = 0;
		if (player == GAME_X) {
			int best_score = -10000000;
			for (uint i = 0; i < moves.size(); i++) {
				if (moves[i].score > best_score) {
					best_move = i;
					best_score = moves[i].score;
				}
			}
		} else {
			int best_score = 10000000;
			for (uint i = 0; i < moves.size(); i++) {
				if (moves[i].score < best_score) {
					best_move = i;
					best_score = moves[i].score;
				}
			}
		}

		return moves[best_move];
	}
} AI;

class Menu : public tgGameState {
public:
	void start() override {
		tgWindow* win = getEngine()->getWindow();
		int w = win->getWidth();
		int h = win->getHeight();

		getECS()->registerSystem(new tgSpriteSystem());
		getECS()->registerSystem(new tgTextSystem());

		getEngine()->setBackColor(tgVector4(1.0f));

		//////// Title
		tgEntity *title = getECS()->create();

		tgTexture *title_tex = tgAssets::getTexture("title.png");
		title_tex->setFilter(GL_NEAREST, GL_NEAREST);
		tgSprite* title_s = title->add<tgSprite>(title_tex);
		title_s->setOrigin(tgVector2(0.5f));
		title_s
			->setup(7, 1)
			->addAnimation("default", {0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,0})
			->play("default", 0.1f, true);

		tgTransform* title_t = title->add<tgTransform>();
		title_t->setLocalPosition(tgVector3(w / 2, -300, 0));
		title_t->setLocalScaling(tgVector3(2.0f));

		tgTweens::addTween(&title_t->getLocalPosition().y(), 100, 2.0f, nullptr, nullptr);

		tgFont *fnt = tgAssets::getFont("ttt_font.fnt");
		fnt->getTexture()->setFilter(GL_NEAREST, GL_NEAREST);

		//////// Play
		tgEntity *play = getECS()->create();
		tgText *play_txt = play->add<tgText>("Play", fnt);
		play_txt->setTextAlign(tgTextAlign::tgALIGN_CENTER);

		tgTransform* play_t = play->add<tgTransform>();
		play_t->setLocalPosition(tgVector3(w/2, h - h / 2, 99));
		play_t->setLocalScaling(tgVector3(2.0f));

		//////// Exit
		tgEntity *exit = getECS()->create();
		tgText *exit_txt = exit->add<tgText>("Exit", fnt);
		exit_txt->setTextAlign(tgTextAlign::tgALIGN_CENTER);

		tgTransform* exit_t = exit->add<tgTransform>();
		exit_t->setLocalPosition(tgVector3(w / 2, (h - h / 2) + 48, 99));
		exit_t->setLocalScaling(tgVector3(2.0f));

		//////// Cursor
		cursor = getECS()->create();
		tgTexture *cursor_tex = tgAssets::getTexture("cursor.png");
		cursor_tex->setFilter(GL_NEAREST, GL_NEAREST);
		tgSprite* cursor_s = cursor->add<tgSprite>(cursor_tex);
		cursor_s->setOrigin(tgVector2(1.0f, 0.5f));

		tgTransform* cursor_t = cursor->add<tgTransform>();
		cursor_t->setLocalPosition(tgVector3(w / 2 - 64, h/2 + 15, 0));
		cursor_t->setLocalScaling(tgVector3(2.0f));
		
		option = 0;
		selected = false;
	}

	void update(float dt) override {
		tgWindow* win = getEngine()->getWindow();
		int w = win->getWidth();
		int h = win->getHeight();

		tgTransform* cursor_t = cursor->get<tgTransform>();
		switch (option) {
			case 0: cursor_t->setLocalPosition(tgVector3(w / 2 - 64, h / 2 + 15, 0)); break;
			case 1: cursor_t->setLocalPosition(tgVector3(w / 2 - 64, (h - h / 2) + 48 + 15, 0)); break;
		}

		if (!selected) {
			if (tgInput::isKeyPressed(SDLK_UP)) {
				if (option-- < 0) {
					option = 1;
				}
			} else if (tgInput::isKeyPressed(SDLK_DOWN)) {
				if (option++ > 1) {
					option = 0;
				}
			}
			if (tgInput::isKeyPressed(SDLK_RETURN)) {
				switch (option) {
					case 0: {
						getEngine()->setState("game");
						getEngine()->setBackColor(tgVector4(0.0f));
					} break;
					case 1: {
						getEngine()->stop();
					} break;
				}

				selected = true;
			}
		}
	}

	int option;
	bool selected;
	tgEntity *cursor;
};

class TicTacToe : public tgGameState {
public:
	void start() override {
		tgWindow* win = getEngine()->getWindow();
		int w = win->getWidth();
		int h = win->getHeight();
		
		getECS()->registerSystem(new tgSpriteSystem());
		getECS()->registerSystem(new tgTextSystem());
		
		//////// Board
		tgEntity *board = getECS()->create();

		tgTexture *board_tex = tgAssets::getTexture("board.png");
		board_tex->setFilter(GL_NEAREST, GL_NEAREST);
		tgSprite* board_s = board->add<tgSprite>(board_tex);
		board_s->setOrigin(tgVector2(0.5f));

		tgTransform* board_t = board->add<tgTransform>();
		board_t->setLocalPosition(tgVector3(w/2, h/2, 0));
		board_t->setLocalScaling(tgVector3(2.0f));

		//////// Text
		tgEntity *text = getECS()->create();
		tgFont *text_fnt = tgAssets::getFont("ttt_font.fnt");
		text_fnt->getTexture()->setFilter(GL_NEAREST, GL_NEAREST);
		tgText *text_text = text->add<tgText>("Score: 0", text_fnt);

		tgTransform* text_t = text->add<tgTransform>();
		text_t->setLocalPosition(tgVector3(30, 30, 99));
		text_t->setLocalScaling(tgVector3(2.0f));

		// Fire particle config
		fire.life = 0.2f;
		fire.angle = -M_PI / 2.0f;
		fire.angleVar = M_PI / 8;
		fire.additive = true;
		fire.angularSpeed = 5.0f;
		fire.angularSpeedVar = -10.0f;
		fire.endColor = fire.endColorVar = tgVector4(1.0f, 0.9f, 0.1f, 1.0f);
		fire.startColor = fire.startColorVar = tgVector4(1.0f, 0.1f, 0.1f, 1.0f);
		fire.startScale = 1.0f;
		fire.startScaleVar = 1.0f;
		fire.endScale = 0.2f;
		fire.endScaleVar = 0.2f;
		fire.speed = 100.0f;
		//

		clearBoard();

		canMakeMove = true;
		currentTurn = GAME_O;
	}

	void update(float dt) override {
		tgWindow* win = getEngine()->getWindow();
		int w = win->getWidth();
		int h = win->getHeight();
		
		// Check if slot was clicked
		if (tgInput::isMouseButtonPressed(1) && currentTurn == GAME_O) {
			if (canMakeMove) {
				int res = board.checkVictory();
				if (res != GAME_O && res != GAME_NULL) {
					canMakeMove = false;
				}
			}
			if (canMakeMove) {
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (board.get(i, j) == GAME_NULL) {
							tgVector2 pos = (piecePositions[j][i] * 2.0f) + tgVector2(w/2-240, 0);
							tgVector4 rect(pos, 112, 66);
							int mx = tgInput::getMouseX();
							int my = tgInput::getMouseY();

							if (mx > rect.x() &&
								mx < rect.x() + rect.z() &&
								my > rect.y() &&
								my < rect.y() + rect.w())
							{
								board.set(i, j, GAME_O);
								addPiece(GAME_O, i, j);
								currentTurn = GAME_X;
								break;
							}
						}
					}
				}
			}
		} else if (currentTurn == GAME_X) {
			int res = board.checkVictory();
			if (res == GAME_NULL) {
				canMakeMove = false;
				tgTimer::wait(0.7f, [&]() {
					Move mv = ai.performMove(board);
					addPiece(GAME_X, mv.x, mv.y);
					canMakeMove = true;
				});
			}
			currentTurn = GAME_O;
		}
	}

	void addPiece(int type, int x, int y) {
		tgWindow* win = getEngine()->getWindow();
		int w = win->getWidth();
		int h = win->getHeight();

		tgEmitter *femit = getEngine()->getParticleEngine()->createEmitter(tgAssets::getTexture("particle.png"));
		femit->setConfig(fire);
		femit->setLoop(false);
		femit->setDuration(0.4f);
		femit->setRenderOrder(10);
		femit->setEmissionRate(70);
		femit->setTransformCallback([this](tgVector2 const& v) {
			float angle = v.x() * (M_PI * 2);
			return tgVector2(std::cos(angle) * 33, std::sin(angle) * 17);
		});
		
		tgEntity *piece = getECS()->create();
		tgTransform *piece_t = piece->add<tgTransform>();

		tgVector2 pos = (piecePositions[y][x] * 2.0f) + tgVector2(w/2-240, -20);
		float py = pos.y();
		piece_t->setLocalPosition(tgVector3(pos, 0.0f));
		piece_t->setLocalScaling(tgVector3(2.0f));

		femit->setPosition(piece_t->getLocalPosition().xy() + tgVector2(56, 33));

		tgTweens::addTween(&piece_t->getLocalPosition().y(), py + 20, 0.4f, nullptr, tgEasing::easeOutBounce);

		tgTexture *tex = nullptr;
		if (type == GAME_O) {
			std::stringstream stm;
			stm << "pieces/o_" << y << "_" << x << ".png";
			tex = tgAssets::getTexture(stm.str());
		} else if (type == GAME_X) {
			std::stringstream stm;
			stm << "pieces/x_" << y << "_" << x << ".png";
			tex = tgAssets::getTexture(stm.str());
		}

		if (tex && board.get(x, y) != GAME_NULL) {
			tex->setFilter(GL_NEAREST, GL_NEAREST);
			piece->add<tgSprite>(tex);
			piece->add<Piece>();
		} else {
			piece->destroy();
		}
	}

	void clearBoard() {
		board.clear();

		for (tgEntity *ent : getECS()->with<Piece>()) {
			ent->destroy();
		}
	}

	void printBoard() {
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				tgLog::print(board.get(x, y), " ");
			}
			tgLog::println("");
		}
	}

	tgParticleConfiguration fire;

	AI ai;
	Board board;

	tgVector2 piecePositions[3][3] = {
		{ tgVector2(35, 57), tgVector2(92, 57), tgVector2(148, 57) },
		{ tgVector2(31, 90), tgVector2(92, 90), tgVector2(152, 90) },
		{ tgVector2(28, 121), tgVector2(92, 121), tgVector2(155, 121) }
	};

	int currentTurn;
	bool canMakeMove;
};

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow("TGE", 640, 480);
	tgEngine *engine = new tgEngine(win);

	engine->registerState("menu", new Menu());
	engine->registerState("game", new TicTacToe());

	engine->start();

	return 0;
}
