/*
 * Copyright (c) 2026. Umamusume Polska
 */

plugins {
    id("buildsrc.convention.kotlin-jvm")
    base
    `jacoco-report-aggregation`
}

dependencies {
    implementation(project(":app"))
    implementation(project(":bluetooth"))
}

tasks.check {
    dependsOn(tasks.named<JacocoReport>("testCodeCoverageReport"))
}