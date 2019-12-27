static void exportData(m3data_s *data) {
    FILE *f = fopen("monster.js", "wt");

    fprintf(f, "var data = [\n");

    for (int i = 0; i < 11; i++) {
        if (i > 0) {
            fprintf(f, ",\n");
        }

        m3mon_s *monster = &m3Mon[i];
        myBYTE *src = data->spi[i];

        fprintf(f, "\t{\n\t\t\"width\": %d,\n\t\t\"height\": %d,\n\t\t\"images\": [\n", monster->Sir, monster->Vys);

        for (int frame = 0; frame < monster->Faz; frame++) {
            if (frame > 0) {
                fprintf(f, ",\n");
            }
            fprintf(f, "\t\t\t[");

            for (int y = 0; y < monster->Vys; y++) {
                for (int x = 0; x < monster->Sir; x++, src++) {
                    if (x + y > 0) {
                        fprintf(f, ", ");
                    }
                    myBYTE color = *src;
                    myBYTE red = data->pal[color][0];
                    myBYTE green = data->pal[color][1];
                    myBYTE blue = data->pal[color][2];
                    myBYTE alpha = color == 0 ? 0 : 255;
                    fprintf(f, "[%d, %d, %d, %d]", red, green, blue, alpha);
                }
            }

            fprintf(f, "]");
        }

        fprintf(f, "\n\t\t]\n\t}");
    }

    fprintf(f, "\n]\n");

    fclose(f);
    exit(0);
}
