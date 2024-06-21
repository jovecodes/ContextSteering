#include "Jovial/Components/Components2D.h"
#include "Jovial/Core/Node.h"
#include "Jovial/JovialEngine.h"
#include "Jovial/Shapes/ShapeDrawer.h"
#include "Jovial/Std/Array.h"

using namespace jovial;

#define WINDOW_NAME "My Jovial Game"
#define WINDOW_SIZE Vector2(1280, 720)
#define WINDOW_RES (WINDOW_SIZE / 4)

class World : public Node {
public:
    World() = default;

    void birth() override {
        rendering::set_aspect_mode(rendering::AspectModes::KeepViewport);
        for (long i = 0; i < weights.length; ++i) {
            weights[i] = 1;
        }
    }

    void update() override {
        float radius = 70;
        Vector2 middle = Window::get_current_vsize() / 2;
        draw_circle_outline({radius + 1, middle}, 1, 32);

        float step = 2 * PI / (float) weights.length;
        float principal = 0;
        for (long i = 0; i < weights.length; ++i) {
            float x = math::cos(principal);
            float y = math::sin(principal);

            Vector2 mouse = middle.direction_to(Input::get_mouse_position());


            // ---------------- STEERING FUNCTIONS ---------------- //

            // weights[i] = Vector2(x, y).dot(mouse); // Chase
            // weights[i] = Vector2(-x, -y).dot(mouse); // Run away
            // weights[i] = 1.0f - math::abs(Vector2(-x, -y).dot(mouse) - 0.65f);// Run away strafe
            weights[i] = math::pow(math::abs(Vector2(-y, x).dot(mouse)), 3); // Left/Right strafe

            // ---------------------------------------------------- //

            Vector2 v = Vector2{x, y} * radius * math::abs(weights[i]) + middle;
            if (weights[i] > 0) {
                draw_line({middle, v}, 1.0, {.color = Color(20, 255, 30, 200)});
            } else {
                draw_line({middle, v}, 1.0, {.color = Color(255, 0, 0, 200)});
            }

            principal += step;
        }
    }

    Array<float, 16> weights = {};
};

int main() {
    Jovial game;

    game.push_plugin(new Window({WINDOW_NAME, WINDOW_SIZE, WINDOW_RES}));
    game.push_plugins(plugins::default_plugins_2d);
    game.push_plugin(new NodePlugin(new World));

    game.run();

    return 0;
}
