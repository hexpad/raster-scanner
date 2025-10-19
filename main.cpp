#include <stdio.h>
#include <math.h>
#include <unistd.h> // for the usleep function

#define OUTPUT_FILE "RasterScan.txt" // Output file
#define NUM_STEPS 10 // Number of steps for each line

// Function that writes coordinates to the file
void writeCoordinatesToFile(FILE *file, double x, double y, double z) {
    fprintf(file, "%.10f, %.10f, %.10f\n", x, y, z);
}

int main() {
    double azimuth_center, elevation_center, width_angle, height_angle, fly_back_time, period;
    int bar_count, pri;
    FILE *outputFile;
    double total_time = 0.0;

    // Get parameters from the user
    printf("Enter period (milliseconds): ");
    scanf("%lf", &period);
    printf("Enter Azimuth Center (degrees): ");
    scanf("%lf", &azimuth_center);
    printf("Enter Elevation Center (degrees): ");
    scanf("%lf", &elevation_center);
    printf("Enter Width Angle (degrees): ");
    scanf("%lf", &width_angle);
    printf("Enter Height Angle (degrees): ");
    scanf("%lf", &height_angle);
    printf("Enter Fly-back Time (milliseconds): ");
    scanf("%lf", &fly_back_time);
    printf("Enter Number of Bars: ");
    scanf("%d", &bar_count);
    printf("Enter PRI (microseconds): ");
    scanf("%d", &pri);

    // Calculate total duration
    total_time = period * (bar_count * bar_count) + (fly_back_time * bar_count);

    // Open the file
    outputFile = fopen(OUTPUT_FILE, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "File could not be opened.\n");
        return 1;
    }

    // Scanning loop
    double elapsed_time = 0.0;
    while (elapsed_time < total_time) {
        // Azimuth scanning
        for (int i = 0; i < bar_count; i++) {
            double azimuth = azimuth_center - (width_angle / 2) + (i * width_angle / (bar_count - 1));
            // Elevation scanning
            for (int j = 0; j < bar_count; j++) {
                double elevation = elevation_center - (height_angle / 2) + (j * height_angle / (bar_count - 1));
                // Generate points equal to the number of steps per line
                for (int k = 0; k < NUM_STEPS; k++) {
                    double step_azimuth = azimuth + k * (width_angle / (bar_count - 1)) / NUM_STEPS;
                    double x = -cos(step_azimuth * M_PI / 180.0) * cos(elevation * M_PI / 180.0); // Invert X coordinate
                    double y = -sin(step_azimuth * M_PI / 180.0) * cos(elevation * M_PI / 180.0); // Invert Y coordinate
                    double z = sin(elevation * M_PI / 180.0);
                    // Write coordinates to file
                    writeCoordinatesToFile(outputFile, x, y, z); // (0,0,0) can be used as the starting point
                }
            }
            // Fly-back duration
            usleep(fly_back_time * 1000);
        }
        // Period duration
        usleep(period * 1000);

        // Update elapsed time
        elapsed_time += period;
    }

    // Close the file
    fclose(outputFile);

    return 0;
}
