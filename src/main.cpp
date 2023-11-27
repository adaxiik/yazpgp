#include <yazpgp/application.hpp>
auto main() -> int
{
    return yazpgp::Application(
        yazpgp::Application::ApplicationConfig{
            .title = "aaaaa",
            .width = 800,
            .height = 450
        }
    )
    .run();
}