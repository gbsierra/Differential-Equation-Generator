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
    std::string P;
    int Q;
    static bool isHomogeneous;
    static bool hasVariableCoefficient;

public:
    // Constructor with user input
    FirstOrderLinearEquation() {
        UpdateInputs();
    }

    void UpdateInputs() {
        P = hasVariableCoefficient ? std::to_string(rand() % 10 + 1) + "x"  : std::to_string(rand() % 10 + 1);

        Q = isHomogeneous ? 0 : rand() % 10 + 1;
    }

    // Generate a string representation of the equation
    std::string toString() override {
        std::string equationType = (isHomogeneous ? "Homogeneous" : "Non-Homogeneous");
        std::string coefficientType = (hasVariableCoefficient ? "Variable Coefficient" : "Constant Coefficient");
        return "Generated First-Order Linear DE\n (" + equationType + ", " + coefficientType + "):\n"
            "dy/dx + " + P + "y = " + std::to_string(Q);
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
        return "Generated Cauchy-Euler Equation:\nx^2 * d^2y/dx^2 + " + std::to_string(a) + "x*dy/dx + " + std::to_string(b) + "y = 0";
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
        return "Generated Higher-Order DE:\nd^2y/dx^2 + " + std::to_string(a) + "dy/dx + " + std::to_string(b) + "y = " + std::to_string(c);
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
        return "Generated Partial DE:\n∂^2u/∂x^2 + " + std::to_string(alpha) + "*∂u/∂x = " + std::to_string(beta);
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
        return "Generated System of Equations:\ndx/dt = " + std::to_string(x_coeff) + "x + " + std::to_string(y_coeff) + "y,\n"
            "dy/dt = " + std::to_string(rhs) + "x";
    }
};

//class ExactEquation
class ExactEquation : public Equation {
private:
    int a, b, c; // coefficients
    int equationType; // (1 for ln, 2 for kx)

public:
    // Constructor
    ExactEquation() {
        a = (rand() % 10 + 1); // Random coefficient a
        b = (rand() % 10 + 1); // Random coefficient b
        c = (rand() % 10 + 1); // Random constant multiplier for kx
        equationType = rand() % 2 + 1; // Choose randomly between 1 and 2
    }

    // Method to represent the equation as a string
    std::string toString() override {
        if (equationType == 1) {
            //type 1: ln-based equation
            return "Generated Exact Equation:\n" +
                std::to_string(a) + "ln(y)*dy + " +
                std::to_string(b) + "ln(x)*dx = 0";
        }
        else {
            //type 2: kx-based equation
            return "Generated Exact Equation:\n" +
                std::to_string(a * c) + "y*dy + " +
                std::to_string(b * c) + "x*dx = 0";
        }
    }
};

//class Seperable Equation
class SeparableEquation : public Equation {
private:
    int p, q; // coefficients for the separable equation

public:
    // Constructor
    SeparableEquation() : p(rand() % 10 + 1), q(rand() % 10 + 1) {}

    // Method to represent the equation as a string
    std::string toString() override {
        return "Generated Separable Equation:\n(dy/" + std::to_string(p) + "y) = (dx/" + std::to_string(q) + "x)";
    }
};

//class Laplace Transform Equation
class LaplaceTransformEquation : public Equation {
private:
    int a, b, c; // coefficients for the differential equation
    int trigChoice; // determines if sin, cos, or both are included

public:
    // Constructor
    LaplaceTransformEquation()
        : a(rand() % 10 + 1), b(rand() % 10 + 1), c(rand() % 10 + 1), trigChoice(rand() % 3) {
    }

    std::string toString() override {
        std::string equation = "Generated Laplace Equation:\n";
        equation += "y'(t) + " + std::to_string(a) + "y(t) = ";

        // Randomly decide to use sin, cos, or both
        if (trigChoice == 0) {
            equation += std::to_string(b) + "sin(" + std::to_string(c) + "t)";
        }
        else if (trigChoice == 1) {
            equation += std::to_string(b) + "cos(" + std::to_string(c) + "t)";
        }
        else {
            equation += std::to_string(b) + "sin(" + std::to_string(c) + "t) + cos(" + std::to_string(c) + "t)";
        }

        return equation;
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
        else if (choice == 6) {
            return std::make_shared<SeparableEquation>();
        }
        else if (choice == 7) {
            return std::make_shared<ExactEquation>();
        }
        else if (choice == 8) {
            return std::make_shared<LaplaceTransformEquation>();
        }
        else {
            std::cout << "Invalid choice. Returning nullptr.\n";
            return nullptr;
        }
    }
};


bool FirstOrderLinearEquation::isHomogeneous = true;  // default value
bool FirstOrderLinearEquation::hasVariableCoefficient = false;  // default value

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

        if (ImGui::BeginPopupModal("First Order Linear", &first_order_helper_window, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Configure Equation Parameters");
            ImGui::Separator();

            FirstOrderLinearEquation* eq = dynamic_cast<FirstOrderLinearEquation*>(current_equation.get());
            if (eq) {
                // Ask if the equation is homogeneous
                ImGui::Checkbox("Homogeneous?", &eq->isHomogeneous);

                // Ask if the equation has variable coefficient
                ImGui::Checkbox("Variable coefficient?", &eq->hasVariableCoefficient);

                // Confirm Button Logic
                if (ImGui::Button("Confirm")) {
                    // Update equation coefficients
                    eq->UpdateInputs();
                    // Close the helper window
                    ImGui::CloseCurrentPopup();
                    first_order_helper_window = false;
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
            float windowWidth = ImGui::GetContentRegionAvail().x; // Available width
            float textWidth = ImGui::CalcTextSize("Select Equation Type:").x; // Width of the text

            ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
            ImGui::Text("Select Equation Type:");

            /*ImGui::Text("test: %d, %d", equation_choice, last_equation_choice);
            FirstOrderLinearEquation* eq = dynamic_cast<FirstOrderLinearEquation*>(current_equation.get());
            if (eq) {
                // Text command reflecting instance data
                ImGui::Text("test: %s, %s",
                    eq->isHomogeneous ? "true" : "false",
                    eq->hasVariableCoefficient ? "true" : "false");
            } testing */

            // Aligning buttons
            const char* labels[] = { "First-Order Linear", "Cauchy-Euler", "Higher-Order", "Partial ", "System of Equations", "Seperable", "Exact", "Laplace Transform"};
            for (int i = 0; i < 8; i++) {
                textWidth = ImGui::CalcTextSize(labels[i]).x;
                ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f - 20.0f);
                ImGui::RadioButton(labels[i], &equation_choice, i + 1);
            }

            if (equation_choice==1 && last_equation_choice!=equation_choice) {
                first_order_helper_window = true; // Open the helper window
            }
            last_equation_choice = equation_choice; // Update the last selection

            // Center and resize the button
            ImGui::SetCursorPosX((windowWidth - 200) * 0.5f); // Hardcode button width as 200
            if (ImGui::Button("Generate Equation", ImVec2(200, 50))) { // Set the size directly
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
            if (current_equation) {
                ImGui::Text("%s", current_equation->toString().c_str());
            }
            else {
                ImGui::Text("No equation generated!");
            }
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

