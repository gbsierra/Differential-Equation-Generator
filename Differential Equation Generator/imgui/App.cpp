#include "App.h"
#include "imgui.h"
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>

//base class for all Equations
class Equation {
public:
    virtual std::string toString() = 0;
    virtual ~Equation() {}
};

//class First-Order DE
class FirstOrderLinearEquation : public Equation {
public:
    int P;
    int Q;
    bool isHomogeneous;
    bool hasVariableCoefficients;

public:
    // Constructor with user input
    FirstOrderLinearEquation() {
        isHomogeneous = false;
        hasVariableCoefficients = false;
        UpdateInputs();
    }

    void UpdateInputs() {
        P = hasVariableCoefficients ? rand() % 10 + 1 : 5; // Variable or constant coefficient for P
        Q = isHomogeneous ? 0 : rand() % 10 + 1; // Non-homogeneous if Q != 0
    }

    // Generate a string representation of the equation
    std::string toString() override {
        std::string equationType = (isHomogeneous ? "Homogeneous" : "Non-Homogeneous");
        std::string coefficientType = (hasVariableCoefficients ? "Variable Coefficients" : "Constant Coefficients");
        return "Generated First-Order Linear DE\n (" + equationType + ", " + coefficientType + "):\n"
            "dy/dx + " + std::to_string(P) + "*y = " + std::to_string(Q);
    }
};

//class Cauchy-Euler Equation
class CauchyEulerEquation : public Equation {
private:
    int a;
    int b;

public:
    // Constructor
    CauchyEulerEquation() : a(rand() % 10 + 1), b(rand() % 10 + 1) {}
    std::string toString() override {
        return "Generated Cauchy-Euler Equation:\nx^2 * d^2y/dx^2 + " + std::to_string(a) + "*x*dy/dx + " + std::to_string(b) + "*y = 0";
    }
};

//class Higher Order Equation
class HigherOrderEquation : public Equation {
private:
    int a, b, c;

public:
    //constructor
    HigherOrderEquation() : a(rand() % 10 + 1), b(rand() % 10 + 1), c(rand() % 10 + 1) {}
    std::string toString() override {
        return "Generated Higher-Order DE:\nd^2y/dx^2 + " + std::to_string(a) + "*dy/dx + " + std::to_string(b) + "*y = " + std::to_string(c);
    }
};

//class Partial Equation
class PartialEquation : public Equation {
private:
    int alpha, beta;

public:
    // Constructor
    PartialEquation() : alpha(rand() % 10 + 1), beta(rand() % 10 + 1) {}

    std::string toString() override {
        return "Generated Partial DE:\n?^2u/?x^2 + " + std::to_string(alpha) + "*?u/?x = " + std::to_string(beta);
    }
};

//class System of Equations
class SystemOfEquations : public Equation {
private:
    int x_coeff, y_coeff, rhs;

public:
    // Constructor
    SystemOfEquations() : x_coeff(rand() % 5 + 1), y_coeff(rand() % 5 + 1), rhs(rand() % 10 + 1) {}

    std::string toString() override {
        return "Generated System of Equations:\ndx/dt = " + std::to_string(x_coeff) + "*x + " + std::to_string(y_coeff) + "*y,\n"
            "dy/dt = " + std::to_string(rhs) + "*x";
    }
};

//class Equation Generator
class EquationGenerator {
public:
    static std::shared_ptr<Equation> generateEquation(int choice) {
        if (choice == 1) {
            return std::make_shared<FirstOrderLinearEquation>();
        }
        else if (choice == 2) {
            return std::make_shared<CauchyEulerEquation>();
        }
        else if (choice == 3) {
            return std::make_shared<HigherOrderEquation>();
        }
        else if (choice == 4) {
            return std::make_shared<PartialEquation>();
        }
        else if (choice == 5) {
            return std::make_shared<SystemOfEquations>();
        }
        else {
            std::cout << "Invalid choice. Returning nullptr.\n";
            return nullptr;
        }
    }
};

// App Namespace for imgui implementation
namespace App {

    //Declarations
    static std::string name = "9th Grade Algebra expert";
    static bool show_demo_window = false;
    static bool equation_selection_window = true;
    static bool equation_display_window = false;
    static bool first_order_helper_window = false;
    static int equation_choice = 2;
    static int last_equation_choice = 0;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void setFirstOrderHelperBool(bool val);
    void setFirstOrderHelperBool(bool val) {
        first_order_helper_window = val;
    }

    // Shared Equation Instance (points to selected equation)
    static std::shared_ptr<Equation> current_equation = std::make_shared<FirstOrderLinearEquation>();

    //
    // Renders First Order Linear Equation Helper Popup
    void RenderFirstOrderParametersWindow() {
        if (first_order_helper_window && std::dynamic_pointer_cast<FirstOrderLinearEquation>(current_equation)) {
            ImGui::OpenPopup("First Order Linear");
        }

        if (ImGui::BeginPopupModal("First Order Linear", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Configure Equation Parameters");
            ImGui::Separator();

            FirstOrderLinearEquation* eq = dynamic_cast<FirstOrderLinearEquation*>(current_equation.get());
            if (eq) {
                // Ask if the equation is homogeneous
                ImGui::Checkbox("Homogeneous?", &eq->isHomogeneous);

                // Ask if the equation has variable coefficients
                ImGui::Checkbox("Variable coefficients?", &eq->hasVariableCoefficients);

                // Confirm Button Logic
                if (ImGui::Button("Confirm")) {
                    // Update equation coefficients
                    eq->UpdateInputs();
                    // Close the helper window
                    first_order_helper_window = false;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
    }

    // Renders Main Equation Generation Window
    void RenderEquationSelectionWindow() {
        if (equation_selection_window) {
            ImGui::SetNextWindowPos(ImVec2(240.0f, 100.0f), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_Always);

            ImGui::Begin("Generate Equation", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
            // Dropdown to select the type of equation to generate
            ImGui::Text("Select Equation Type:");
            ImGui::RadioButton("First-Order Linear", &equation_choice, 1);
            //ImGui::SameLine();
            ImGui::RadioButton("Cauchy-Euler", &equation_choice, 2);
            //ImGui::SameLine();
            ImGui::RadioButton("Higher-Order", &equation_choice, 3);
            //ImGui::SameLine();
            ImGui::RadioButton("Partial", &equation_choice, 4);
            //ImGui::SameLine();
            ImGui::RadioButton("System of Equations", &equation_choice, 5);

            if (equation_choice == 1 && last_equation_choice != equation_choice) {
                first_order_helper_window = true; // Open the helper window
            }
            last_equation_choice = equation_choice; // Update the last selection

            // Generate Equation Button
            if (ImGui::Button("Generate Equation")) {
                current_equation = EquationGenerator::generateEquation(equation_choice); // Update the equation
                equation_display_window = true;
            }

            ImGui::End();
        }
    }

    // Renders Equation Output
    void RenderEquationDisplayWindow() {
        if (equation_display_window) {
            ImGui::Begin("Equation Display", &equation_display_window, ImGuiWindowFlags_AlwaysAutoResize);

            // Display the generated equation
            ImGui::Text("%s", current_equation->toString().c_str());

            // Close Button for the display window
            if (ImGui::Button("Close")) {
                equation_display_window = false;
            }

            ImGui::End();
        }
    }

    // Render the Welcome Window
    void RenderWelcomeWindow() {
        static int counter = 0;
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        ImGui::SetNextWindowPos(ImVec2(10, 430));
        ImGui::Begin("Welcome");
        ImGui::Text("Welcome, %s", name.c_str());
        ImGui::Checkbox("Demo Window", &show_demo_window);

        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::End();
    }


    // Main Render Function for UI
    void RenderUI() {
        RenderFirstOrderParametersWindow(); // Render the helper popup
        RenderEquationSelectionWindow();     // Render the equation generator window
        RenderEquationDisplayWindow();
        RenderWelcomeWindow();      // Render the welcome window
    }
}

