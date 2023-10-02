#include <yazpgp/application.hpp>
int main(int argc, const char** argv)
{
    yazpgp::Application app(
        yazpgp::Application::ApplicationConfig{
            .title = "aaaaa",
            .width = 800,
            .height = 450
        }
    );
    return app.run();
}