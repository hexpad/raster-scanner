#include <stdio.h>
#include <math.h>
#include <unistd.h> // for the usleep function

#define OUTPUT_FILE "RasterScan.txt"
#define NUM_STEPS 10

void writeCoordinatesToFile(FILE *file, double x, double y, double z) {
    fprintf(file, "%.10f, %.10f, %.10f\n", x, y, z);
}

int main() {
    double aziCenter, eleCenter, widthAng, heightAng, fbTime, prd;
    int bars, pri;
    FILE *outputFile;
    double totalTime = 0.0;

    printf("Enter period (milliseconds): ");
    scanf("%lf", &prd);
    printf("Enter Azimuth Center (degrees): ");
    scanf("%lf", &aziCenter);
    printf("Enter Elevation Center (degrees): ");
    scanf("%lf", &eleCenter);
    printf("Enter Width Angle (degrees): ");
    scanf("%lf", &widthAng);
    printf("Enter Height Angle (degrees): ");
    scanf("%lf", &heightAng);
    printf("Enter Fly-back Time (milliseconds): ");
    scanf("%lf", &fbTime);
    printf("Enter Number of Bars: ");
    scanf("%d", &bars);
    printf("Enter PRI (microseconds): ");
    scanf("%d", &pri);

    totalTime = prd * (bars * bars) + (fbTime * bars);

    outputFile = fopen(OUTPUT_FILE, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "File could not be opened.\n");
        return 1;
    }

    double timeElapsed = 0.0;
    while (timeElapsed < totalTime) {
        for (int i = 0; i < bars; i++) {
            double az = aziCenter - (widthAng / 2) + (i * widthAng / (bars - 1));
            for (int j = 0; j < bars; j++) {
                double el = eleCenter - (heightAng / 2) + (j * heightAng / (bars - 1));
                for (int k = 0; k < NUM_STEPS; k++) {
                    double stepAz = az + k * (widthAng / (bars - 1)) / NUM_STEPS;
                    double x = -cos(stepAz * M_PI / 180.0) * cos(el * M_PI / 180.0);
                    double y = -sin(stepAz * M_PI / 180.0) * cos(el * M_PI / 180.0);
                    double z = sin(el * M_PI / 180.0);
                    writeCoordinatesToFile(outputFile, x, y, z);
                }
            }
            usleep(fbTime * 1000);
        }
        usleep(prd * 1000);
        timeElapsed += prd;
    }

    fclose(outputFile);

    return 0;
}

